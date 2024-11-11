#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

std::ostream& LX::Function::Print(std::ostream& os) const
{
    if (Export) os << "export ";
    if (Extern) os << "extern ";
    os << (Name.empty() ? "$" : Name) << '(';
    for (size_t i = 0; i < Params.size(); ++i)
    {
        if (i > 0) os << ", ";
        Params[i].Print(os);
    }
    os << ')';
    if (!Type->Result()->IsVoid())
        Type->Result()->Print(os << " => ");
    if (Body) Body->Print(os << " = ");
    return os;
}

LX::ValuePtr LX::Function::GenIR(const SourceLocation& where, Builder& builder) const
{
    llvm::Function* function;
    ValuePtr result;

    if (!Name.empty() && builder.Contains(Name))
    {
        result = builder.Get(where, Name);
        function = llvm::dyn_cast<llvm::Function>(result->Load(builder));
    }
    else
    {
        const auto type = llvm::dyn_cast<llvm::FunctionType>(Type->GenIR(builder));
        const auto name = Export && !Extern
                              ? builder.ModuleId() + '.' + Name
                              : Name;

        function = llvm::Function::Create(
            type,
            Export || Extern
                ? llvm::GlobalValue::ExternalLinkage
                : llvm::GlobalValue::InternalLinkage,
            name,
            builder.IRModule());
        result = RValue::Create(builder.Ctx().GetPointerType(Type), function);

        if (!Name.empty())
            builder.Define(where, Name) = result;
    }

    if (!Body) return result;
    if (!function->empty())
        Error(where, "cannot redefine function '{}'", Name);

    const auto unit = builder.DIBuilder().createFile(builder.DIUnit().getFilename(), builder.DIUnit().getDirectory());
    const auto sub = builder.DIBuilder().createFunction(
        unit,
        function->getName(),
        Name,
        unit,
        where.Row,
        llvm::dyn_cast<llvm::DISubroutineType>(Type->GenDI(builder)),
        0,
        llvm::DINode::FlagPrototyped,
        llvm::DISubprogram::SPFlagDefinition);
    function->setSubprogram(sub);

    builder.Push(sub);
    const auto bkp = builder.IRBuilder().GetInsertBlock();
    builder.IRBuilder().SetInsertPoint(llvm::BasicBlock::Create(builder.IRContext(), "entry", function));
    for (size_t i = 0; i < Params.size(); ++i)
    {
        const auto& [param_type_, param_name_] = Params[i];

        const auto arg = function->getArg(i);
        arg->setName(param_name_);

        builder.Define(where, param_name_) =
            param_type_->IsMutable()
                ? LValue::Create(param_type_->Element(), arg)
                : RValue::Create(param_type_, arg);

        const auto d = builder.DIBuilder().createParameterVariable(
            sub,
            param_name_,
            i + 1,
            unit,
            where.Row,
            param_type_->GenDI(builder));
        builder.DIBuilder().insertDeclare(
            arg,
            d,
            builder.DIBuilder().createExpression(),
            llvm::DILocation::get(
                builder.IRContext(),
                where.Row,
                where.Col,
                sub),
            builder.IRBuilder().GetInsertBlock());
    }

    auto value = Body->GenIR(builder);
    if (Type->Result()->IsVoid())
        builder.IRBuilder().CreateRetVoid();
    else
    {
        value = builder.Cast(where, value, Type->Result());
        builder.IRBuilder().CreateRet(
            Type->Result()->IsMutable()
                ? value->Ptr()
                : value->Load(builder));
    }
    builder.IRBuilder().SetInsertPoint(bkp);
    builder.Pop();

    builder.RunPasses(*function);
    return result;
}
