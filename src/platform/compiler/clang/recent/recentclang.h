// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_RECENTCLANG
#define QOR_PP_H_RECENTCLANG

#include <string>

namespace qor { namespace compiler {

    class RecentClangCompiler
    {

    public:
    
        RecentClangCompiler() = default;
        virtual ~RecentClangCompiler() = default;

        template< typename T>
        static std::string demangle()
        {
            std::string name("clang not supporting this yet"/*typeid(T).name()*/);
            const std::string struct_prefix("struct ");
            const std::string class_prefix("class ");
            const std::string ptr_postfix(" *");

            std::string::size_type at = name.find(struct_prefix);
            if (at != std::string::npos) 
            { 
                name.erase(at, struct_prefix.size()); 
            }
            at = name.find(class_prefix);
            if (at != std::string::npos) 
            { 
                name.erase(at, class_prefix.size()); 
            }
            at = name.find(ptr_postfix);
            if (at != std::string::npos) 
            { 
                name.erase(at, ptr_postfix.size()); 
            }
            return name;
        }

    };

    typedef RecentClangCompiler CompilerBase;

}}//qor::compiler

#endif//QOR_PP_H_RECENTCLANG
