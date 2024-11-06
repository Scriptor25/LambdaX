#include <LX/AST.hpp>
#include <LX/Type.hpp>

#include "LX/Builder.hpp"
#include "LX/Context.hpp"

LX::ConstStructExpr::ConstStructExpr(std::vector<ExprPtr> elements, TypePtr type)
    : Elements(std::move(elements)), Type(std::move(type))
{
}

std::ostream& LX::ConstStructExpr::Print(std::ostream& os) const
{
    os << "{ ";
    for (size_t i = 0; i < Elements.size(); ++i)
    {
        if (i > 0) os << ", ";
        Elements[i]->Print(os);
    }
    os << " }";
    if (Type) Type->Print(os << " => ");
    return os;
}

void LX::ConstStructExpr::GenIR(Builder& builder, Value& ref) const
{
    std::vector<Value> values;
    std::vector<Parameter> elements;
    std::vector<llvm::Constant*> constants;
    for (size_t i = 0; i < Elements.size(); ++i)
    {
        auto& value = values.emplace_back();
        Elements[i]->GenIR(builder, value);
        if (Type) builder.Cast(value, Type->Element(i), value);
        auto& [type, name] = elements.emplace_back();
        type = value.Type;
        if (const auto c = llvm::dyn_cast<llvm::Constant>(value.ValueIR))
            constants.push_back(c);
    }

    const auto type = Type ? Type : builder.Ctx().GetStructType(elements);
    const auto type_ir = type->GetIR(builder);

    ref.Type = type;
    if (constants.size() == values.size())
    {
        ref.ValueIR = llvm::ConstantStruct::get(llvm::dyn_cast<llvm::StructType>(type_ir), constants);
        return;
    }

    ref.ValueIR = llvm::UndefValue::get(type_ir);
    for (size_t i = 0; i < values.size(); ++i)
        ref.ValueIR = builder.IRBuilder().CreateInsertValue(ref.ValueIR, values[i].ValueIR, i);
}
