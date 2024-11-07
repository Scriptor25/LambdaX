#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>

LX::ConstStructExpr::ConstStructExpr(TypePtr type, std::vector<ExprPtr> elements)
    : Expr(std::move(type)), Elements(std::move(elements))
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
    std::vector<llvm::Constant*> constants;

    for (size_t i = 0; i < Elements.size(); ++i)
    {
        auto& value = values.emplace_back();
        value = Elements[i]->GenIR(builder);
        value = builder.Cast(value, Type->Element(i));
        if (const auto c = llvm::dyn_cast<llvm::Constant>(value->Load(builder)))
            constants.push_back(c);
    }

    const auto type_ir = Type->GetIR(builder);

    if (constants.size() == values.size())
    {
        const auto value = llvm::ConstantStruct::get(llvm::dyn_cast<llvm::StructType>(type_ir), constants);
        return RValue::Create(Type, value);
    }

    llvm::Value* value = llvm::UndefValue::get(type_ir);
    for (size_t i = 0; i < values.size(); ++i)
        value = builder.IRBuilder().CreateInsertValue(value, values[i]->Load(builder), i);
    return RValue::Create(Type, value);
}
