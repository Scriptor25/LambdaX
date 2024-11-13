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
        TypePtr& GetVoidType();
        TypePtr& GetIntType(unsigned, bool);
        TypePtr& GetFloatType(unsigned);
        TypePtr& GetStructType(const SourceLocation&, const std::string&, const std::vector<Parameter>&);
        TypePtr& GetPointerType(const TypePtr&);
        TypePtr& GetMutableType(const TypePtr&);
        TypePtr& GetArrayType(const TypePtr&, size_t);
        TypePtr& GetFunctionType(const TypePtr&, const std::vector<Parameter>&, bool);
        TypePtr& GetFunPtrType(const TypePtr&, const std::vector<Parameter>&, bool);

    private:
        std::map<std::string, TypePtr> m_Types;
    };
}
