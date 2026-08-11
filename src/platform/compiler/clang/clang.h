// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

/*Define here the things that are intrinsic to all clang compilers and specific to
clang compilers. Anything specific to a subset of clang versions or common to all
compilers should be elsewhere.
*/

#define qor_pp_compiler_at __FILE__ "(" qor_pp_stringize(__LINE__)") : "

#ifdef _WIN64
#   define WINCALL
#else
#   define WINCALL __stdcall
#endif

#define qor_pp_compiler_debugbreak(e)

static constexpr int function_base = 0;
static constexpr int function_stride = 1;

//Calling conventions
#define qor_pp_compiler_callcon						__cdecl
#define qor_pp_compiler_stdcallconvention			__stdcall
#define qor_pp_compiler_thiscall					//explicit __thiscall not supported? not sure why you'd need it anyway?
#define qor_pp_compiler_fastcall					__fastcall

#define qor_pp_export			__attribute__((__visibility__("default")))
#define qor_pp_import			
#define qor_pp_thread_local     __thread
#define qor_pp_forceinline		[[clang::always_inline]]
#define qor_pp_noinline			__attribute__((noinline))
#define qor_pp_funcsig          __PRETTY_FUNCTION__
#define qor_pp_allocator        [[clang::allocator]]

#define qor_pp_assume(_X)       __builtin_assume(_X)
//c++ 23 #define qor_pp_assume(_X)       [[assume(_X)]]

#define qor_pp_cpu_cache_line   64
