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
        TypePtr& GetMutableType(const TypePtr&);
        TypePtr& GetFunctionType(const TypePtr&, const std::vector<Parameter>&, bool);
        TypePtr& GetFunPtrType(const TypePtr&, const std::vector<Parameter>&, bool);

        void Push();
        void Pop();
        TypePtr& DefVar(const std::string&);
        TypePtr GetVar(const std::string&);
        bool HasVar(const std::string&);

    private:
        std::map<std::string, TypePtr> m_Types;
        std::vector<std::map<std::string, TypePtr>> m_Stack;
    };
}
