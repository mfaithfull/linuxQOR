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

extern "C" __declspec(dllimport) int WINCALL IsDebuggerPresent();
extern "C" __declspec(dllimport) void WINCALL DebugBreak();
#define qor_pp_compiler_debugbreak(e) if (IsDebuggerPresent()) DebugBreak(); else (void)0

#ifdef __EDG__
static constexpr int function_base = 3;
static constexpr int function_stride = 2;
#else
static constexpr int function_base = 0;
static constexpr int function_stride = 1;
#endif

#define _SILENCE_NONFLOATING_COMPLEX_DEPRECATION_WARNING

//Calling conventions
#define qor_pp_compiler_callcon						__cdecl
#define qor_pp_compiler_stdcallconvention			__stdcall
#define qor_pp_compiler_thiscall					//explicit __thiscall not supported? not sure why you'd need it anyway?
#define qor_pp_compiler_fastcall					__fastcall

#define qor_pp_export			__declspec(dllexport)
#define qor_pp_import			__declspec(dllimport)
#define qor_pp_thread_local     __declspec(thread)
#define qor_pp_forceinline		inline
#define qor_pp_noinline			__declspec(noinline)
#define qor_pp_funcsig          __FUNCSIG__
#define qor_pp_allocator        __declspec(allocator)

#define qor_pp_assume(X)        __assume(X)

#define qor_pp_cpu_cache_line   std::hardware_destructive_interference_size

//Disable this annoyance for now as this is a development library intended to be built by the 'end' user against whaetver STL they have
#pragma warning( disable : 4251 ) // class ... needs to have dll-interface to be used by clients of class ..
#pragma warning( disable : 4275 ) //non dll-interface class ... used as base for dll-interface class
