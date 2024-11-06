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
        TypePtr& GetStructType(const std::vector<Parameter>&);
        TypePtr& GetPointerType(const TypePtr&);
        TypePtr& GetFunctionType(const TypePtr&, const std::vector<TypePtr>&, bool);
        TypePtr& GetFunPtrType(const TypePtr&, const std::vector<TypePtr>&, bool);

    private:
        std::map<std::string, TypePtr> m_Types;
    };
}
