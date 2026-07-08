// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#define qor_pp_compiler_at __FILE__ ":" qor_pp_stringize(__LINE__)": "
#define qor_pp_compiler_debugbreak(e)

static constexpr int function_base = 0;
static constexpr int function_stride = 1;

#define qor_pp_compiler_extra_destructor

//Calling conventions
#define qor_pp_compiler_callcon						__attribute__ ((cdecl))
#define qor_pp_compiler_stdcallconvention			__attribute__ ((stdcall)
#define qor_pp_compiler_thiscall					__attribute__ ((thiscall))
#define qor_pp_compiler_fastcall					__attribute__ ((fastcall))

#define qor_pp_export           __attribute__((__visibility__("default")))
#define qor_pp_import
#define qor_pp_thread_local     __thread
#define qor_pp_forceinline      inline __attribute__((always_inline))
#define qor_pp_noinline			__attribute__ ((noinline))
#define qor_pp_funcsig          __PRETTY_FUNCTION__
#define qor_pp_allocator        

#define qor_pp_assume(X)        

#define qor_pp_cpu_cache_line   64