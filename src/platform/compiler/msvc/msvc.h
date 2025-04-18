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

/*Define here the things that are intrinsic to all MSVC compilers and specific to
MSVC compilers. Anything specific to a subset of MSVC versions or common to all
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

#define qor_pp_export			__declspec(dllexport)
#define qor_pp_import			__declspec(dllimport)
#define qor_pp_thread_local     __declspec(thread)
#define qor_pp_forceinline		inline
#define qor_pp_noinline			__declspec(noinline)
#define qpr_pp_funcsig          __FUNCSIG__
#define qor_pp_allocator        __declspec(allocator)