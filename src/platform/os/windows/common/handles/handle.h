// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_OS_WINDOWS_COMMON_HANDLE_HANDLE
#define QOR_PP_H_PLATFORM_OS_WINDOWS_COMMON_HANDLE_HANDLE

namespace qor { namespace platform { namespace win {

	class qor_pp_module_interface(QOR_WINCOMMON) PrimitiveHandle
	{
	public:

		PrimitiveHandle();
		PrimitiveHandle(void* h);
		PrimitiveHandle(int h);
		virtual ~PrimitiveHandle();
		PrimitiveHandle(const PrimitiveHandle&);
		PrimitiveHandle& operator = (PrimitiveHandle && src);
		PrimitiveHandle& operator = (const PrimitiveHandle&);
		void* operator()();
		virtual void* Use(void) const;

		void* ptr(void) const
		{
			return m_h;
		}

		void** pptr(void)
		{
			return &m_h;
		}

		bool operator == (const PrimitiveHandle & cmp) const
		{
			return (m_h == cmp.m_h) ? true : false;
		}

		bool operator < (const PrimitiveHandle & cmp) const
		{
			return m_h < cmp.m_h;
		}

	protected:

		void* m_h{nullptr};

	};

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
		void DontClose();
		bool NeedsClose();
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

		void* m_h{nullptr};
		bool m_needsClose{false};
	};

}}}//qor::platform::win

#endif//QOR_PP_H_PLATFORM_OS_WINDOWS_COMMON_HANDLE_HANDLE