#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::CallExpr::CallExpr(ExprPtr callee, std::vector<ExprPtr> args)
    : Callee(std::move(callee)), Args(std::move(args))
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
    if (!callee.Type->IsPointer()) Error("callee is not a pointer");

    const auto callee_type = callee.Type->Element();
    if (!callee_type) Error("cannot create call on opaque pointer");
    if (!callee_type->IsFunction()) Error("callee is not a function pointer");
    if (Args.size() != callee_type->ParamCount()) Error("incorrect number of arguments");

    const auto type_ir = llvm::dyn_cast<llvm::FunctionType>(callee_type->GenIR(builder));

    std::vector<llvm::Value*> args(Args.size());
    for (size_t i = 0; i < Args.size(); ++i)
    {
        Value arg;
        Args[i]->GenIR(builder, arg);
        builder.Cast(arg, callee_type->Param(i), arg);
        args[i] = arg.ValueIR;
    }

    ref.Type = callee_type->Result();
    ref.ValueIR = builder.IRBuilder().CreateCall(type_ir, callee.ValueIR, args);
}
