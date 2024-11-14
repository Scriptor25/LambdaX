#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

#include "LX/Context.hpp"

std::ostream& LX::GlobalSymbol::Print(std::ostream& os) const
{
    if (Export) os << "export ";
    if (Extern) os << "extern ";
    if (IsMutable) os << "mut ";
    os << Name << ' ';
    if (Type) Type->Print(os << "=> ");
    if (Init) Init->Print(os << "= ");
    return os;
}

LX::ValuePtr LX::GlobalSymbol::GenIR(const SourceLocation& where, Builder& builder) const
{
    static unsigned priority = 0;

    const auto linkage = Export || Extern
                             ? llvm::GlobalValue::ExternalLinkage
                             : llvm::GlobalValue::InternalLinkage;
    const auto name = Export && !Extern
                          ? builder.ModuleId() + '.' + Name
                          : Name;
    const auto external = !Init && !IsMutable;

    llvm::Function* ctor{};

    ValuePtr init;
    if (Init)
    {
        const auto type = builder.Ctx().GetFunctionType(builder.Ctx().GetVoidType(), {}, false, false);
        const auto ctor_type = llvm::dyn_cast<llvm::FunctionType>(type->GenIR(where, builder));
        ctor = llvm::Function::Create(
            ctor_type,
            llvm::GlobalValue::InternalLinkage,
            {},
            builder.IRModule());

        const auto di_file = builder.DIBuilder().createFile(
            builder.DIUnit().getFilename(),
            builder.DIUnit().getDirectory());
        const auto sub = builder.DIBuilder().createFunction(
            di_file,
            ctor->getName(),
            Name,
            di_file,
            where.Row,
            llvm::dyn_cast<llvm::DISubroutineType>(type->GenDI(builder)),
            where.Row,
            llvm::DINode::FlagPrototyped,
            llvm::DISubprogram::SPFlagDefinition);
        ctor->setSubprogram(sub);

        builder.IRBuilder().SetCurrentDebugLocation({});
        builder.Push(sub);

        const auto entry_block = llvm::BasicBlock::Create(builder.IRContext(), "entry", ctor);
        builder.IRBuilder().SetInsertPoint(entry_block);

        init = Init->GenIR(builder);
        if (Type) init = builder.CreateCast(where, init, Type);
    }

    const auto type = Type ? Type : init->Type();
    const auto ir_type = type->GenIR(where, builder);

    const auto global_var = new llvm::GlobalVariable(
        builder.IRModule(),
        ir_type,
        false,
        linkage,
        llvm::Constant::getNullValue(ir_type),
        name,
        nullptr,
        llvm::GlobalValue::NotThreadLocal,
        std::nullopt,
        external);

    const auto value = LValue::Create(type, global_var, IsMutable);

    if (Init)
    {
        const auto ir_init = init->Load(where, builder);
        value->StoreForce(where, builder, ir_init);
        builder.IRBuilder().CreateRetVoid();
        builder.IRBuilder().SetCurrentDebugLocation({});
        builder.IRBuilder().ClearInsertionPoint();
        builder.RunPasses(*ctor);
        builder.Pop();

        const auto ir_int_type = builder.IRBuilder().getInt32Ty();
        const auto ir_ptr_type = builder.IRBuilder().getPtrTy();
        auto ir_struct_type = llvm::StructType::getTypeByName(builder.IRContext(), "ctor.type");
        if (!ir_struct_type)
            ir_struct_type = llvm::StructType::create(
                builder.IRContext(),
                {ir_int_type, ir_ptr_type, ir_ptr_type},
                "ctor.type");

        const auto ir_priority = builder.IRBuilder().getInt32(priority++);
        const auto ir_data = llvm::Constant::getNullValue(ir_ptr_type);
        const auto ir_struct = llvm::ConstantStruct::get(ir_struct_type, {ir_priority, ctor, ir_data});

        std::vector<llvm::Constant*> ctors;
        if (const auto global_ctors = builder.IRModule().getGlobalVariable("llvm.global_ctors"))
        {
            const auto existing = llvm::dyn_cast<llvm::ConstantArray>(global_ctors->getInitializer());
            for (unsigned i = 0; i < existing->getNumOperands(); ++i)
                ctors.push_back(existing->getOperand(i));
            global_ctors->eraseFromParent();
        }
        ctors.push_back(ir_struct);

        const auto ir_array_type = llvm::ArrayType::get(ir_struct_type, ctors.size());
        const auto ir_array = llvm::ConstantArray::get(ir_array_type, ctors);

        new llvm::GlobalVariable(
            builder.IRModule(),
            ir_array_type,
            true,
            llvm::GlobalValue::AppendingLinkage,
            ir_array,
            "llvm.global_ctors");
    }

    return builder.Define(where, Name) = value;
}
