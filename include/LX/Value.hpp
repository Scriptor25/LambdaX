#pragma once

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <LX/LX.hpp>

namespace LX
{
    struct Value
    {
        bool operator!() const;

        TypePtr Type;
        llvm::Value* ValueIR{};
    };
}
