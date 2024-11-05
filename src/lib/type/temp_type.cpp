#include <LX/Error.hpp>
#include <LX/Type.hpp>

LX::TempType::TempType(std::string name)
    : Type(std::move(name), 0)
{
}

llvm::Type* LX::TempType::GenIR(Builder&) const
{
    Error("cannot generate template type");
}
