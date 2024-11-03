#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::CallExpr::CallExpr(TypePtr type, ExprPtr callee, std::vector<ExprPtr> args)
    : Expr(std::move(type)), Callee(std::move(callee)), Args(std::move(args))
{
}

std::ostream& LX::CallExpr::Print(std::ostream& os) const
{
    Callee->Print(os) << '(';
    for (size_t i = 0; i < Args.size(); ++i)
    {
        if (i > 0) os << ", ";
        Args[i]->Print(os);
    }
    return os << ')';
}

void LX::CallExpr::GenIR(Builder& builder, Value& ref) const
{
    Value callee;
    Callee->GenIR(builder, callee);
    if (!callee) Error("callee is null");
    const auto callee_type = llvm::dyn_cast<llvm::FunctionType>(callee.Type->Element()->GenIR(builder));

    std::vector<llvm::Value*> args(Args.size());
    for (size_t i = 0; i < Args.size(); ++i)
    {
        Value arg;
        Args[i]->GenIR(builder, arg);
        builder.Cast(arg, callee.Type->Element()->Param(i), arg);
        args[i] = arg.ValueIR;
    }

    ref.Type = Type;
    ref.TypeIR = Type->GenIR(builder);
    ref.ValueIR = builder.IRBuilder().CreateCall(callee_type, callee.ValueIR, args);
}
