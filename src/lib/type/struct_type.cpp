#include <llvm/IR/DerivedTypes.h>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Parameter.hpp>
#include <LX/Type.hpp>

std::string LX::StructType::GetName(const std::vector<Parameter>& elements)
{
    std::string str = "{ ";
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

LX::StructType::StructType(std::vector<Parameter> elements)
    : Type(GetName(elements), GetBits(elements)), Elements(std::move(elements))
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

llvm::Type* LX::StructType::GenIR(Builder& builder) const
{
    std::vector<llvm::Type*> elements(Elements.size());
    for (size_t i = 0; i < Elements.size(); ++i)
        elements[i] = Elements[i].Type->GetIR(builder);
    return llvm::StructType::get(builder.IRContext(), elements);
}