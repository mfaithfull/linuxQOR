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

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_MOCKREPLACE
#define QOR_PP_H_MOCKREPLACE

#if (qor_pp_arch_target == qor_pp_arch_anyX86)
#	if (qor_pp_compiler == qor_pp_compiler_msvc) && (qor_pp_os_target == qor_pp_os_windows)
#		define qor_pp_mock_cfuncsupport
#	elif (qor_pp_os_target == qor_pp_os_linux) && (qor_pp_compiler == qor_pp_compiler_gcc)
#		define qor_pp_mock_cfuncsupport
#	elif defined(qor_pp_os_target == qor_pp_os_mac)
#		define qor_pp_mock_cfuncsupport
#	endif
#elif (qor_pp_arch_target == qor_pp_arch_anyARM) && (qor_pp_compiler == qor_pp_compiler_gcc)
#	define qor_pp_mock_cfuncsupport
extern "C" void __clear_cache(char* beg, char* end);		// This clear-cache is *required*. The tests will fail if you remove it.
#endif

#ifdef qor_pp_mock_cfuncsupport

#  include <memory.h>

#if (qor_pp_os_target == qor_pp_os_windows)
// De-windows.h-ified import to avoid including that file.
#	ifdef qor_pp_os_64bit
extern "C" __declspec(dllimport) int WINCALL VirtualProtect(void* func, unsigned long long byteCount, unsigned long flags, unsigned long* oldFlags);
#	else
extern "C" __declspec(dllimport) int WINCALL VirtualProtect(void* func, unsigned long byteCount, unsigned long flags, unsigned long* oldFlags);
#   endif
#	ifndef PAGE_EXECUTE_READWRITE
#		define PAGE_EXECUTE_READWRITE 0x40
#	endif
#endif

//--------------------------------------------------------------------------------
namespace qor 
{
	//--------------------------------------------------------------------------------
	namespace mock 
	{
#if (qor_pp_os_target == qor_pp_os_windows)        
		//--------------------------------------------------------------------------------
		class Unprotect
		{
		public:

			//--------------------------------------------------------------------------------
			Unprotect(void* location, size_t byteCount) : origFunc(location) , byteCount(byteCount)
			{
				VirtualProtect(origFunc, byteCount, PAGE_EXECUTE_READWRITE, &oldprotect);
			}

			//--------------------------------------------------------------------------------
			~Unprotect()
			{
				unsigned long dontcare;
				VirtualProtect(origFunc, byteCount, oldprotect, &dontcare);
			}

		private:
			void* origFunc;
			size_t byteCount;
			unsigned long oldprotect;
		};
#  else
#   include <sys/mman.h>
#   include <stdint.h>
		//--------------------------------------------------------------------------------
		class Unprotect
		{
		public:
			Unprotect(void* location, size_t count)
				: origFunc((intptr_t)location& (~0xFFF))
				, byteCount(count + ((intptr_t)location - origFunc))
			{
				mprotect((void*)origFunc, this->byteCount, PROT_READ | PROT_WRITE | PROT_EXEC);
			};
			~Unprotect()
			{
				mprotect((void*)origFunc, byteCount, PROT_READ | PROT_EXEC);
			}
		private:
			intptr_t origFunc;
			int byteCount;
		};
#  endif

		typedef unsigned int e9ptrsize_t;

		//--------------------------------------------------------------------------------
		template <typename T, typename U>
		T horrible_cast(U u)
		{
			union { T t; U u; } un;
			un.u = u;
			return un.t;
		}

		//--------------------------------------------------------------------------------
		class Replace
		{
		private:
			void* origFunc;
			char backupData[16]; // typical use is 5 for 32-bit and 14 for 64-bit code.
		public:

			//--------------------------------------------------------------------------------
			template <typename T>
			Replace(T funcptr, T replacement) : origFunc(horrible_cast<void*>(funcptr))
			{
				Unprotect _allow_write(origFunc, sizeof(backupData));
				memcpy(backupData, origFunc, sizeof(backupData));
#if (qor_pp_arch_target == qor_pp_arch_anyX86)
#	ifdef qor_pp_arch_is_64bit
				if (llabs((long long)origFunc - (long long)replacement) < 0x80000000LL) 
				{
#	endif
					* (unsigned char*)origFunc = 0xE9;
					e9ptrsize_t temp_address = (e9ptrsize_t)(horrible_cast<intptr_t>(replacement) - horrible_cast<intptr_t>(origFunc) - sizeof(e9ptrsize_t) - 1);
					memcpy((e9ptrsize_t*)(horrible_cast<intptr_t>(origFunc) + 1), &temp_address, sizeof(e9ptrsize_t));
#   ifdef qor_pp_arch_is_64bit
				}
				else 
				{
					unsigned char* func = (unsigned char*)origFunc;
					func[0] = 0xFF; // jmp (rip + imm32)
					func[1] = 0x25;
					func[2] = 0x00; // imm32 of 0, so immediately after the instruction
					func[3] = 0x00;
					func[4] = 0x00;
					func[5] = 0x00;
					*(long long*)(horrible_cast<intptr_t>(origFunc) + 6) = (long long)(horrible_cast<intptr_t>(replacement));
				}
#	endif
#elif (qor_pp_arch_target == qor_pp_arch_anyARM)
				unsigned int* rawptr = (unsigned int*)((intptr_t)(origFunc) & (~3));
				if ((intptr_t)origFunc & 1) 
				{
					rawptr[0] = 0x6800A001;
					rawptr[1] = 0x46874687;
					rawptr[2] = (intptr_t)replacement;
				}
				else 
				{
					rawptr[0] = 0xE59FF000;
					rawptr[1] = (intptr_t)replacement;
					rawptr[2] = (intptr_t)replacement;
				}
				__clear_cache((char*)rawptr, (char*)rawptr + 16);
#endif
			}

			//--------------------------------------------------------------------------------
			~Replace()
			{
				Unprotect _allow_write(origFunc, sizeof(backupData));
				memcpy(origFunc, backupData, sizeof(backupData));
#if (qor_pp_arch_target == qor_pp_arch_anyARM)
				unsigned int* rawptr = (unsigned int*)((intptr_t)(origFunc) & (~3));
				__clear_cache((char*)rawptr, (char*)rawptr + 16);
#endif
			}
		};

	}//mock
}//qor

# endif//qor_pp_mock_cfuncsupport


#endif//QOR_PP_H_MOCKREPLACE
