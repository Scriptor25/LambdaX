#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::CallExpr::CallExpr(SourceLocation where, ExprPtr callee, std::vector<ExprPtr> args)
    : Expr(std::move(where)), Callee(std::move(callee)), Args(std::move(args))
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

LX::ValuePtr LX::CallExpr::GenIR(Builder& builder) const
{
    const auto callee = Callee->GenIR(builder);
    if (!callee->Type()->IsPointer()) Error(Where, "callee is not a pointer");
    const auto callee_type = callee->Type()->Element();
    if (!callee_type) Error(Where, "cannot create call on opaque pointer");
    if (!callee_type->IsFunction()) Error(Where, "callee is not a function pointer");
    if (Args.size() < callee_type->ParamCount()) Error(Where, "not enough arguments");
    if (!callee_type->HasVarArg() && Args.size() > callee_type->ParamCount()) Error(Where, "too many arguments");

    std::vector<llvm::Value*> args(Args.size());
    for (size_t i = 0; i < Args.size(); ++i)
    {
        auto arg = Args[i]->GenIR(builder);

        const auto param =
            i < callee_type->ParamCount()
                ? callee_type->Param(i)
                : nullptr;

        if (param && !param->IsMutable() && arg->Type() != param)
            arg = builder.CreateCast(Where, arg, param);

        if (param && param->IsMutable())
        {
            if (!arg->IsMutable())
                Error(Where, "cannot assign immutable value to mutable arg");
            if (param->Element() != arg->Type())
                Error(Where, "cannot assign value of type {} to mutable arg of type {}", arg->Type(), param);
            args[i] = arg->Ptr();
        }
        else args[i] = arg->Load(builder);
    }

    Where.EmitDI(builder);

    const auto type_ir = llvm::dyn_cast<llvm::FunctionType>(callee_type->GenIR(builder));

    const auto type = callee_type->Result();
    const auto value = builder.IRBuilder().CreateCall(type_ir, callee->Load(builder), args);
    return type->IsMutable()
               ? LValue::Create(type->Element(), value, true)
               : RValue::Create(type, value);
}
