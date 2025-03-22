// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <string>
#include <cxxabi.h>

#define qor_pp_do_pragma(x) _Pragma (#x)
#define qor_pp_todo(x) qor_pp_do_pragma(message ("TODO - " #x))
#define qor_pp_warning(x) qor_pp_do_pragma(warning ("Warning - " #x))
#define qor_pp_message(x) qor_pp_do_pragma(message ("Message - " #x))

qor_pp_message(Compiling with GCC 9.4.0)

namespace qor { namespace compiler {

    class GCC9_4_0
    {
    private:

        std::string static inline interpret_demangle_result(int status, std::string& result)
        {
            switch (status)
            {
            case 0:
                return result;
            break;
            case 1:
                result = "No result from demangler";
            break;
            case -1:
                result = "memory allocation failure in demangler";
            break;
            case -2:
                result = "MANGLED_NAME is not a valid name under the C++ ABI mangling rules";
            break;
            case -3:
                result = "One of the arguments is invalid";
            break;
            default:
                result = "Incomprehensible demangler status";
            break;
            } 
            return result;
        }

    public:
    
        template< typename T>
        static std::string demangle()
        {
            int status = 1;
            char* demangled = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
            std::string result(demangled);
            free(demangled);
            return interpret_demangle_result(status, result);
        }
    };

    typedef GCC9_4_0 CompilerBase;

}}//qor::compiler