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
    if (!Type->Result({})->IsVoid())
        Type->Result({})->Print(os << " => ");
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
        function = llvm::dyn_cast<llvm::Function>(result->Load(where, builder));
    }
    else
    {
        const auto type_ir = llvm::dyn_cast<llvm::FunctionType>(Type->GenIR(where, builder));
        const auto linkage = Export || Extern
                                 ? llvm::GlobalValue::ExternalLinkage
                                 : llvm::GlobalValue::InternalLinkage;
        const auto name = Export && !Extern
                              ? builder.ModuleId() + '.' + Name
                              : Name;

        function = llvm::Function::Create(type_ir, linkage, name, builder.IRModule());
        result = RValue::Create(builder.Ctx().GetPointerType(Type), function);

        if (!Name.empty())
            builder.Define(where, Name) = result;
    }

    if (!Body) return result;
    if (!function->empty())
        Error(where, "redefining function '{}'", Name);

    const auto unit = builder.DIBuilder().createFile(
        builder.DIUnit().getFilename(),
        builder.DIUnit().getDirectory());
    const auto sub = builder.DIBuilder().createFunction(
        unit,
        function->getName(),
        Name,
        unit,
        where.Row,
        llvm::dyn_cast<llvm::DISubroutineType>(Type->GenDI(builder)),
        where.Row,
        llvm::DINode::FlagPrototyped,
        llvm::DISubprogram::SPFlagDefinition);
    function->setSubprogram(sub);

    builder.IRBuilder().SetCurrentDebugLocation({});
    builder.Push(sub);

    const auto bkp = builder.IRBuilder().GetInsertBlock();
    builder.IRBuilder().SetInsertPoint(llvm::BasicBlock::Create(builder.IRContext(), "entry", function));

    for (size_t i = 0; i < Params.size(); ++i)
    {
        const auto& [mutable_, type_, name_] = Params[i];

        const auto arg = function->getArg(i);
        arg->setName(name_);

        auto& var = builder.Define(where, name_);
        if (type_->IsReference())
            var = LValue::Create(type_->Element(where), arg, mutable_);
        else
        {
            var = builder.CreateAlloca(where, type_, mutable_);
            var->StoreForce(where, builder, arg);
        }

        const auto d = builder.DIBuilder().createParameterVariable(
            sub,
            name_,
            i + 1,
            unit,
            where.Row,
            var->Type()->GenDI(builder),
            true);
        builder.DIBuilder().insertDeclare(
            var->Ptr(where),
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
    if (Type->Result(where)->IsVoid())
        builder.IRBuilder().CreateRetVoid();
    else
    {
        value = builder.CreateCast(where, value, Type->Result(where));
        builder.IRBuilder().CreateRet(
            Type->Result(where)->IsReference()
                ? value->Ptr(where)
                : value->Load(where, builder));
    }

    builder.IRBuilder().SetCurrentDebugLocation({});
    builder.IRBuilder().SetInsertPoint(bkp);

    builder.Pop();

    builder.RunPasses(*function);
    return result;
}
