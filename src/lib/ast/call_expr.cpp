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
    const auto callee_type = callee->Type()->Element(Where);
    if (!callee_type->IsFunction()) Error(Where, "callee is not a function pointer");
    if (Args.size() < callee_type->ParamCount(Where)) Error(Where, "not enough arguments");
    if (!callee_type->HasVarArg(Where) && Args.size() > callee_type->ParamCount(Where))
        Error(Where, "too many arguments");

    std::vector<llvm::Value*> args(Args.size());
    for (size_t i = 0; i < Args.size(); ++i)
    {
        auto arg = Args[i]->GenIR(builder);
        const auto param =
            i < callee_type->ParamCount(Where)
                ? callee_type->Param(Where, i)
                : nullptr;

        if (param && !param->IsMutable() && arg->Type() != param)
            arg = builder.CreateCast(Where, arg, param);

        if (param && param->IsMutable())
        {
            if (!arg->IsMutable())
                Error(Where, "assigning immutable value to mutable arg index {}", i);
            if (param->Element(Where) != arg->Type())
                Error(
                    Where,
                    "assigning value of type {} to mutable arg type {}, index {}; casting would make it immutable",
                    arg->Type(),
                    i,
                    param);
            args[i] = arg->Ptr(Where);
        }
        else args[i] = arg->Load(Where, builder);
    }

    Where.EmitDI(builder);

    const auto type_ir = llvm::dyn_cast<llvm::FunctionType>(callee_type->GenIR(Where, builder));

    const auto type = callee_type->Result(Where);
    const auto value = builder.IRBuilder().CreateCall(type_ir, callee->Load(Where, builder), args);
    return type->IsMutable()
               ? LValue::Create(type->Element(Where), value, true)
               : RValue::Create(type, value);
}
