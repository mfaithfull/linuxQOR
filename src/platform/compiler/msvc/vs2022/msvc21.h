// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MSVC21
#define QOR_PP_H_MSVC21

#include <string>

namespace qor { namespace compiler {

    class MSVCCompiler21
    {

    public:
    
        inline MSVCCompiler21() = default;
        inline virtual ~MSVCCompiler21() = default;

        template< typename T>
        static std::string demangle()
        {
            std::string name(typeid(T).name());
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

    typedef MSVCCompiler21 CompilerBase;

}}//qor::compiler

#endif//QOR_PP_H_MSVC21
