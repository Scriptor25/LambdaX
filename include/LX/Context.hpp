#pragma once

#include <map>
#include <string>
#include <vector>
#include <LX/LX.hpp>

namespace LX
{
    class Context
    {
        struct StackFrame
        {
            TypePtr& operator[](const std::string&);
            const TypePtr& operator[](const std::string&) const;
            [[nodiscard]] bool contains(const std::string&) const;

            std::map<std::string, TypePtr> Variables;
        };

    public:
        Context();

        TypePtr& GetType(const std::string&);
        TypePtr GetIntType(unsigned, bool);
        TypePtr GetPointerType(const TypePtr&);
        TypePtr GetFunctionType(const TypePtr&, const std::vector<TypePtr>&);
        TypePtr GetFunPtrType(const TypePtr&, const std::vector<TypePtr>&);

        void Push();
        void Pop();
        TypePtr& DefVar(const std::string&);
        const TypePtr& GetVar(const std::string&);

    private:
        std::map<std::string, TypePtr> m_Types;
        std::vector<StackFrame> m_Stack;
    };
}
