// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPILER
#define QOR_PP_H_COMPILER       

#include <string>
#include <cstdint>
#ifndef qor_pp_compiler_include
#   error Every translation unit that uses QOR headers must first include 'src/configuration/configuration.h'
#endif
#include qor_pp_compiler_include

#ifndef qor_pp_compiler_at
#   error Compiler support must provide a definition for qor_pp_compiler_at
#endif

namespace qor { namespace compiler {

    class Compiler : public CompilerBase
    {
    public:
        virtual ~Compiler() = default;

        const char* Name();
    };

    qor_pp_module_interface(QOR_COMPILER) extern const Compiler* TheCompiler();

    template <typename T>
    static std::string demangle()
    {
        return TheCompiler()->demangle<T>();
    }
    
}}//qor::compiler


namespace qor{
    typedef uint8_t byte;
}//qor

#endif//QOR_PP_H_COMPILER
