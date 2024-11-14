#include <llvm/IR/DerivedTypes.h>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Parameter.hpp>
#include <LX/Type.hpp>

std::string LX::StructType::GetName(const std::string& name, const std::vector<Parameter>& elements)
{
    if (!name.empty()) return "struct " + name;

    std::string str = "struct { ";
    for (size_t i = 0; i < elements.size(); ++i)
    {
        if (i > 0) str += ", ";
        str += elements[i].Type->Name;
    }
    return str + " }";
}

unsigned LX::StructType::GetBits(const std::vector<Parameter>& elements)
{
    unsigned bits = 0;
    for (const auto& [mutable_, type_, name_] : elements)
        bits += type_->Bits;
    return bits;
}

LX::StructType::StructType(std::string name, std::vector<Parameter> elements)
    : Type(GetName(name, elements), GetBits(elements)), StructName(std::move(name)), Elements(std::move(elements))
{
}

bool LX::StructType::IsStruct() const
{
    return true;
}

LX::Parameter& LX::StructType::Element(const SourceLocation&, const size_t i)
{
    return Elements[i];
}

size_t LX::StructType::IndexOf(const SourceLocation& where, const std::string& name)
{
    for (size_t i = 0; i < Elements.size(); ++i)
        if (Elements[i].Name == name)
            return i;
    Error(where, "no element '{}' in type {}", name, Name);
}

void LX::StructType::SetElements(const SourceLocation& where, const std::vector<Parameter>& elements)
{
    if (!Elements.empty())
    {
        size_t i = 0;
        if (Elements.size() == elements.size())
            for (; i < Elements.size(); ++i)
                if (Elements[i].Type != elements[i].Type)
                    break;
        if (i < Elements.size())
            Error(where, "redefining non-opaque type {} with different elements", Name);
        return;
    }
    Elements = elements;
}

llvm::Type* LX::StructType::GenIR(const SourceLocation& where, Builder& builder)
{
    if (m_IR) return m_IR;

    std::vector<llvm::Type*> elements(Elements.size());
    for (size_t i = 0; i < Elements.size(); ++i)
        elements[i] = Elements[i].Type->GenIR(where, builder);

    if (StructName.empty())
        return llvm::StructType::get(builder.IRContext(), elements);

    if (const auto type = llvm::StructType::getTypeByName(builder.IRContext(), StructName))
    {
        if (elements.empty()) return type;
        if (type->isOpaque()) type->setBody(elements);
        return type;
    }

    if (elements.empty())
        return llvm::StructType::create(builder.IRContext(), StructName);

    return llvm::StructType::create(builder.IRContext(), elements, StructName);
}

llvm::DIType* LX::StructType::GenDI(Builder& builder)
{
    constexpr auto dead_beef = 0xDEADBEEF;
    if (m_DI == reinterpret_cast<llvm::DIType*>(dead_beef))
        return {};

    if (m_DI) return m_DI;
    m_DI = reinterpret_cast<llvm::DIType*>(dead_beef);

    const auto di_file = llvm::DIFile::get(
        builder.IRContext(),
        builder.DIUnit().getFilename(),
        builder.DIUnit().getDirectory());

    std::vector<llvm::Metadata*> elements;
    size_t offset = 0;
    for (const auto& [mutable_, type_, name_] : Elements)
    {
        elements.push_back(builder.DIBuilder().createMemberType(
            &builder.DIUnit(),
            name_,
            di_file,
            0,
            type_->Bits,
            0,
            offset,
            llvm::DINode::FlagZero,
            type_->GenDI(builder)));
        offset += type_->Bits;
    }

    return m_DI = builder.DIBuilder().createStructType(
        &builder.DIUnit(),
        StructName,
        di_file,
        0,
        Bits,
        0,
        llvm::DINode::DIFlags::FlagZero,
        nullptr,
        builder.DIBuilder().getOrCreateArray(elements));
}
