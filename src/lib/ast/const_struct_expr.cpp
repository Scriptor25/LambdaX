#include <LX/AST.hpp>
#include <LX/Builder.hpp>
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
    std::vector<llvm::Constant*> constants;

    const auto get_e = [&](const size_t i)
    {
        return Type->IsStruct()
                   ? Type->Element(Where, i).Type
                   : Type->Base(Where);
    };

    for (size_t i = 0; i < Elements.size(); ++i)
    {
        auto& value = values.emplace_back();
        value = Elements[i]->GenIR(builder);
        value = builder.CreateCast(Where, value, get_e(i));
        if (const auto c = llvm::dyn_cast<llvm::Constant>(value->Load(Where, builder)))
            constants.push_back(c);
    }

    Where.EmitDI(builder);

    const auto type_ir = Type->GenIR(Where, builder);
    if (constants.size() == values.size())
    {
        llvm::Constant* value{};
        if (Type->IsStruct())
            value = llvm::ConstantStruct::get(llvm::dyn_cast<llvm::StructType>(type_ir), constants);
        else value = llvm::ConstantArray::get(llvm::dyn_cast<llvm::ArrayType>(type_ir), constants);
        return RValue::Create(Type, value);
    }

    llvm::Value* value = llvm::UndefValue::get(type_ir);
    for (size_t i = 0; i < values.size(); ++i)
        value = builder.IRBuilder().CreateInsertValue(value, values[i]->Load(Where, builder), i);
    return RValue::Create(Type, value);
}
