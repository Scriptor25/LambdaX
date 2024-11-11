#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::ConstStructExpr::ConstStructExpr(SourceLocation where, std::vector<ExprPtr> elements, TypePtr type)
    : Expr(std::move(where)), Elements(std::move(elements)), Type(std::move(type))
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

LX::ValuePtr LX::ConstStructExpr::GenIR(Builder& builder) const
{
    std::vector<ValuePtr> values;
    std::vector<Parameter> params;
    std::vector<llvm::Constant*> constants;

    for (size_t i = 0; i < Elements.size(); ++i)
    {
        auto& value = values.emplace_back();
        value = Elements[i]->GenIR(builder);
        if (Type)
            value = builder.Cast(Where, value, Type->Element(i));
        else params.emplace_back(value->Type(), "");
        if (const auto c = llvm::dyn_cast<llvm::Constant>(value->Load(builder)))
            constants.push_back(c);
    }

    Where.EmitDI(builder);

    const auto type = Type ? Type : builder.Ctx().GetStructType("", params);
    const auto type_ir = type->GenIR(builder);

    if (constants.size() == values.size())
    {
        const auto value = llvm::ConstantStruct::get(llvm::dyn_cast<llvm::StructType>(type_ir), constants);
        return RValue::Create(type, value);
    }

    llvm::Value* value = llvm::UndefValue::get(type_ir);
    for (size_t i = 0; i < values.size(); ++i)
        value = builder.IRBuilder().CreateInsertValue(value, values[i]->Load(builder), i);
    return RValue::Create(type, value);
}
