// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SRC_PLATFORM_OS_WINDOWS_API_LAYER_LIBRARY
#define QOR_PP_H_SRC_PLATFORM_OS_WINDOWS_API_LAYER_LIBRARY

#include <excpt.h>
#include "src/qor/flyers/error/error.h"

namespace qor { namespace win { namespace api {

	class Library
	{
	public:

		typedef int(__stdcall *DefProc)(void);//Default procedure pointer type

		static int ExceptionFilterFunction(LPEXCEPTION_POINTERS exception_pointers)
		{
			serious("Win32 API Exception: Code {0}, Address {1}, Flags {2}, Info {3}, {4} parameters.",
				exception_pointers->ExceptionRecord->ExceptionCode,
				exception_pointers->ExceptionRecord->ExceptionAddress,
				exception_pointers->ExceptionRecord->ExceptionFlags,
				exception_pointers->ExceptionRecord->ExceptionInformation[0],
				exception_pointers->ExceptionRecord->NumberParameters
			);
			return EXCEPTION_EXECUTE_HANDLER;
		}

		//Every entry point reached through DefProc is a WINAPI (__stdcall) export of a Win32 system DLL -
		//the 1638 qor_pp_useswinapi/AW bindings in this tree name user32, kernel32, gdi32, ole32, ws2_32,
		//shell32, comctl32, uxtheme, opengl32, dwmapi, Xinput1_4 and shcore only. The pointer the call is
		//made through must therefore carry the same convention as DefProc itself; a default (__cdecl) fPtr
		//makes the call site clean up the stack after a callee that already cleaned it up with "ret N",
		//which on Win32/x86 pops the return address. The mismatch is invisible on x64, where there is a
		//single calling convention, and no existing check could see it because Library::Call has no
		//prototype to compare against. Measured with tmp/_tmp_u6_cc_probe.cpp built for x86 by
		//tmp/_tmp_u6_cc_x86.bat: before the fix the disassembly of Call emitted "call dword ptr [...]"
		//followed by "add esp,0Ch"; after it, no stack adjustment. See open item U6.
		//inlineCall/voidInlineCall below stay unqualified deliberately - their argument is the address of an
		//SDK FORCEINLINE definition (winbase.h:7915 InitializeThreadpoolEnvironment and its five siblings), which the
		//SDK declares with the default convention.
		template< typename ret, class ...MethodArgs >
		static ret Call(const DefProc pProc, MethodArgs... args)
		{
			typedef ret(__stdcall *fPtr)(MethodArgs...);//__stdcall like DefProc, not the default __cdecl

			fPtr FP = reinterpret_cast<fPtr>(pProc);

			if (FP == nullptr)
			{
				throw("Missing library function.");
			}
			__try
			{
				return (FP)(std::forward< MethodArgs >(args)...);
			}
			__except(ExceptionFilterFunction(GetExceptionInformation()))
			{
			}
			ret _{};
			return _;
		}

		template< class ...MethodArgs  >
		static void voidCall(const DefProc pProc, MethodArgs... args)
		{
			typedef void(__stdcall *fPtr)(MethodArgs...);//__stdcall like DefProc, not the default __cdecl

			fPtr FP = reinterpret_cast<fPtr>(pProc);

			if (FP != nullptr)
			{
				__try
				{
					(FP)(std::forward< MethodArgs >(args)...);
				}
				__except(ExceptionFilterFunction(GetExceptionInformation()))
				{
				}
			}
			else
			{
				throw("Missing library function.");
			}
		}

		// Variant (I) binding for header-inline Windows API entry points - see DECISIONS.md D11.
		// WinBase.h and winnt.h define these entry points FORCEINLINE and there is no DLL export for them, so the
		// DefProc path above cannot be used: GetProcAddress returns null for these names and Call/voidCall then throw
		// "Missing library function." at run time. These two helpers take the address of the header definition instead,
		// so the binding happens at compile time and the call emits no import. Verified with tmp/_tmp_inline_probe_v2.cpp,
		// which compiles all eight callback-environ calls with zero undefined Threadpool or Tp* external symbols.
		// Invoke with explicit template arguments, like the export-bound wrappers, and prefix the name with :: because
		// the SDK definitions are at global scope while the QOR declarations live in qor::win::api.
		//
		template< typename ret, class ...MethodArgs >
		static ret inlineCall(ret (*pFn)(MethodArgs...), MethodArgs... args)
		{
			__try
			{
				return (pFn)(std::forward< MethodArgs >(args)...);
			}
			__except(ExceptionFilterFunction(GetExceptionInformation()))
			{
			}
			ret _{};
			return _;
		}

		template< class ...MethodArgs >
		static void voidInlineCall(void (*pFn)(MethodArgs...), MethodArgs... args)
		{
			__try
			{
				(pFn)(std::forward< MethodArgs >(args)...);
			}
			__except(ExceptionFilterFunction(GetExceptionInformation()))
			{
			}
		}

	};

}}}//qor::win::api

#endif//QOR_PP_H_SRC_PLATFORM_OS_WINDOWS_API_LAYER_LIBRARY
