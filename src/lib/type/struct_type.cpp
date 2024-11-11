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
    for (const auto& [type, name] : elements)
        bits += type->Bits;
    return bits;
}

LX::StructType::StructType(std::string name, std::vector<Parameter> elements)
    : Type(GetName(name, elements), GetBits(elements)), StructName(std::move(name)), Elements(std::move(elements))
{
}

LX::TypePtr LX::StructType::Element(const size_t i) const
{
    return Elements[i].Type;
}

size_t LX::StructType::IndexOf(const std::string& name) const
{
    for (size_t i = 0; i < Elements.size(); ++i)
        if (Elements[i].Name == name)
            return i;
    Error("no element '{}' in type {}", name, Name);
}

void LX::StructType::PutElements(const std::vector<Parameter>& elements)
{
    if (!Elements.empty())
    {
        size_t i = 0;
        if (Elements.size() == elements.size())
            for (; i < Elements.size(); ++i)
                if (Elements[i].Type != elements[i].Type)
                    break;
        if (i < Elements.size())
            Error("redefining non-opaque type {} with different elements", Name);
        return;
    }
    Elements = elements;
}

llvm::Type* LX::StructType::GenIR(Builder& builder) const
{
    std::vector<llvm::Type*> elements(Elements.size());
    for (size_t i = 0; i < Elements.size(); ++i)
        elements[i] = Elements[i].Type->GenIR(builder);

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

llvm::DIType* LX::StructType::GenDI(Builder& builder) const
{
    std::vector<llvm::DINode*> elements;
    for (const auto& [type_, name_] : Elements)
        elements.push_back(type_->GenDI(builder));

    return builder.DIBuilder().createStructType(
        &builder.DIScope(),
        StructName,
        llvm::DIFile::get(
            builder.IRContext(),
            builder.DIScope().getFilename(),
            builder.DIScope().getDirectory()),
        0,
        Bits,
        0,
        llvm::DINode::DIFlags::FlagZero,
        nullptr,
        {});
}
