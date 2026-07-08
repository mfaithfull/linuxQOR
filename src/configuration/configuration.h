// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

/*The root internal header file for the entire QOR project
* This must be the first include in every translation unit
*/

#ifndef QOR_PP_H_CONFIGURATION
#define QOR_PP_H_CONFIGURATION

#define qor_pp_profiling_enabled
#define qor_pp_configindex_unicode 63
#define qor_pp_unicode 1
#include <cstdint>
#include <string>

#include "../platform/os/systems.h"
#include "../platform/architecture/architectures.h"

//NOTE: Set preprocessor options for how the build proceeds here or predef them in the build script
#ifndef NDEBUG
//#   define qor_pp_compiler_reportconfig             //Choose to get output during compilation indicating configurations chosen and detected
//#   define qor_pp_compiler_reportdefecits           //Choose to get output during compilation of features unavailable in the toolchain
#endif
//#define qor_pp_os_target qor_pp_os_windows          //Define the target Operating System
//#define qor_pp_arch_target qor_pp_arch_anyX86       //Define the target hardware architecture


#include "../platform/compiler/detecttoolchain.h"           //Detect the preprocessor/compiler/linker/loader toolchain and configure for it
#include "../platform/architecture/detectarchitecture.h"    //Determine the target arch by defaulting to the host arch if the target hasn't been predefined
#include "../platform/os/detectos.h"                        //Determine the target OS by defaulting to the host OS if the target hasn't been predefined

#define qor_pp_module_interface( _X ) qor_pp_if(qor_pp_is_empty(_X), qor_pp_import, qor_pp_export)
#define qor_pp_module_external( _X ) qor_pp_if(qor_pp_is_empty(_X), qor_pp_export, qor_pp_import)
#define qor_pp_module_requires(_X) bool qor_pp_cat(requires,_X) = qor::qor_pp_cat(Implements,_X)();
#define qor_pp_module_will_provide(_M, _X) namespace qor{ bool qor_pp_module_interface(_M) qor_pp_cat(Implements,_X)(); }
#define qor_pp_module_provide(_M, _X) namespace qor{ bool qor_pp_module_interface(_M) qor_pp_cat(Implements,_X)(){ return true; } }//qor

#if (qor_pp_compiler == qor_pp_compiler_gcc)
#define qor_pp_module_interface_gcc( _X ) qor_pp_module_interface(_X)
#else
#define qor_pp_module_interface_gcc( _X )
#endif

#if (qor_pp_unicode && (qor_pp_os_target == qor_pp_os_windows))
typedef char16_t char_t;
#else
typedef char char_t;
#endif

typedef std::basic_string<char> string_t;

#define qor_pp_use_x 0
#define qor_pp_use_wayand 1

namespace qor {
    constexpr uint64_t qor_config = 0 | ( static_cast<unsigned long long>(qor_pp_unicode) << qor_pp_configindex_unicode);
}//qor

#endif//QOR_PP_H_CONFIGURATION
