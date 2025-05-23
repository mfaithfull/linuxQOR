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

#ifndef QOR_PP_H_PLATFORM_OS_WINDOWS_COMMON_HANDLE_HANDLE
#define QOR_PP_H_PLATFORM_OS_WINDOWS_COMMON_HANDLE_HANDLE

namespace qor { namespace nsWindows {

	//class over a Windows Handle
	class qor_pp_module_interface(QOR_WINCOMMON) Handle
	{
	public:

		Handle();
		Handle(void* h);
		Handle(int h);
		virtual ~Handle();
		Handle(const Handle&);
		Handle& operator = (Handle && src);
		Handle& operator = (const Handle&);
		Handle Clone(void* sourceProcessHandle, void* targetProcessHandle, unsigned long desiredAccess, bool inheritHandle, unsigned long options);
		virtual void Attach(void* pObj);
		virtual void* Detach(void);
		bool Duplicate(void* sourceProcessHandle, void* targetProcessHandle, void** targetHandle, unsigned long desiredAccess, bool inheritHandle, unsigned long options);
		bool GetHeritable();
		bool GetProtectFromClose();
		bool SetHeritable(bool inherit);
		bool SetProtectFromClose(bool close);
		virtual void Close(void);
		bool IsNull(void) const;
		bool IsInvalid(void) const;
		void Drop(void);
		void* Use(void) const;

		void* ptr(void) const
		{
			return m_h;
		}

		void** pptr(void)
		{
			return &m_h;
		}

		bool operator == (const Handle & cmp) const
		{
			return (m_h == cmp.m_h) ? true : false;
		}

		bool operator < (const Handle & cmp) const
		{
			return m_h < cmp.m_h;
		}

	protected:

		bool GetInformation(unsigned long* flags);
		bool SetInformation(unsigned long mask, unsigned long flags);

		void* m_h;
		bool m_needsClose;
	};

}}//qor::nsWindows

#endif//QOR_PP_H_PLATFORM_OS_WINDOWS_COMMON_HANDLE_HANDLE