#pragma once

#include <map>
#include <string>
#include <vector>
#include <LX/LX.hpp>

namespace LX
{
    class Context
    {
    public:
        Context();

        TypePtr& GetType(const std::string&);
        TypePtr& GetIntType(unsigned, bool);
        TypePtr& GetFloatType(unsigned);
        TypePtr& GetPointerType(const TypePtr&);
        TypePtr& GetFunctionType(const TypePtr&, const std::vector<TypePtr>&);
        TypePtr& GetFunPtrType(const TypePtr&, const std::vector<TypePtr>&);

    private:
        std::map<std::string, TypePtr> m_Types;
    };
}
