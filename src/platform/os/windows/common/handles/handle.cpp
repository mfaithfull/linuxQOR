// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "handle.h"
#include "src/qor/flyers/error/error.h"

#include "src/platform/os/windows/common/constants.h"
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

using namespace qor::win::api;

namespace qor { namespace platform { namespace win {

	PrimitiveHandle::PrimitiveHandle() : m_h(nullptr)
	{
	}

	PrimitiveHandle::PrimitiveHandle(void* h) : m_h(h)
	{
	}

	PrimitiveHandle::PrimitiveHandle(int h) : m_h((void*)(static_cast<size_t>(h)))
	{
	}

	//U20 - this warning fires for handles a consumer already released through the family's own verb, and for ones it never owned.
	//(Note landed by the winqor_api_user review; its entry is src/platform/os/windows/api_layer/user/registers/u20.md,
	//opened 30 Oct 2026 (session 9, cursor.cpp) and closed 11 Nov 2026 by decision D47. The cites are that entry's,
	//carried as written - nothing in this file was re-audited for them.)
	//PrimitiveHandle stores a value; ownership is not this class's to know. There is no release or reset API
	//(handle.h:9-47), so a consumer that released the handle cannot clear m_h before the member dies - and the release
	//verb is per family, not Close: HCURSOR is DestroyCursor, HICON DestroyIcon, HWND DestroyWindow (user32.h:152),
	//HDC ReleaseDC (:339), HMENU DestroyMenu (:227), HENHMETAFILE DeleteEnhMetaFile (gdi32.h:251) while CloseEnhMetaFile
	//(gdi32.h:245) creates one, HIMAGELIST ImageList_Destroy, HHEAP HeapDestroy. "Must release", "may release" and
	//"must not release" are three states, and this class cannot tell them apart - which is exactly the ambiguity the
	//(bool own) on the Cursor/Icon ctors is trying to carry.
	//The sites that do: ui/gui/gdiobjects/cursor.cpp:55-62 and ui/gui/gdiobjects/icon.cpp:58-64 release through the
	//family verb and leave m_handle set, so this warning is raised both for an owning object whose handle was already
	//destroyed and for a non-owning object whose handle was never its own to destroy. windowfactory.cpp:89/90/92 makes
	//it happen on every WindowFactory::AddWindowClass call, the review's only live path for this pair of classes.
	//Recorded, not fixed: giving this class a reset/release changes winqor_wincommon's published interface and the
	//ownership contract of every PrimitiveHandle consumer - a behaviour change in another library, which the api_layer
	//review records rather than performs (the rule U20 states, closed by D47).


	PrimitiveHandle::~PrimitiveHandle()
	{
		if(m_h != nullptr)
		{
			warning("PrimitiveHandle destructor called with non-null handle. This may indicate a resource leak.");
		}
	}

	PrimitiveHandle::PrimitiveHandle(const PrimitiveHandle& src) : m_h(src.m_h)
	{
	}

	PrimitiveHandle& PrimitiveHandle::operator = (PrimitiveHandle && src)
	{
		m_h = src.m_h;
		return *this;
	}

	PrimitiveHandle& PrimitiveHandle::operator = (const PrimitiveHandle& src)
	{
		m_h = src.m_h;
		return *this;
	}

	void* PrimitiveHandle::operator()()
	{
		return m_h;
	}

	void* PrimitiveHandle::Use(void) const
	{
		return m_h;
	}



	Handle::Handle() : m_h(Invalid_Handle_Value), m_needsClose(false) {}

	Handle::Handle(void* h) : m_h(h)
	{
		m_needsClose = !GetProtectFromClose();
	}

	Handle::Handle(int h) : m_h((void*)(static_cast<size_t>(h))), m_needsClose(false) {}

	Handle::~Handle()
	{
		if (m_needsClose)
		{
			Close();
		}
		m_h = nullptr;
	}

	Handle::Handle(const Handle& src)
	{
		*this = src;
	}

	Handle& Handle::operator = (const Handle& src)
	{
		if (&src != this)
		{
			m_h = src.m_h;
			m_needsClose = false;
		}
		return *this;
	}

	Handle& Handle::operator = (Handle&& src)
	{
		if (&src != this)
		{
			m_h = src.m_h;
			src.m_h = nullptr;
			m_needsClose = src.m_needsClose;
			src.m_needsClose = false;
		}
		return *this;
	}

	Handle Handle::Clone(void* sourceProcessHandle, void* targetProcessHandle, unsigned long desiredAccess, bool inheritHandle, unsigned long options)
	{
		void* newHandle = nullptr;
		Kernel32::DuplicateHandle(sourceProcessHandle, m_h, targetProcessHandle, &newHandle, desiredAccess, inheritHandle ? TRUE : FALSE, options);
		Handle h(newHandle);
		h.m_needsClose = m_needsClose;
		return h;
	}

	bool Handle::Duplicate(void* sourceProcessHandle, void* targetProcessHandle, void** targetHandle, unsigned long desiredAccess, bool inheritHandle, unsigned long options)
	{
		bool result = Kernel32::DuplicateHandle(sourceProcessHandle, m_h, targetProcessHandle, targetHandle, desiredAccess, inheritHandle ? TRUE : FALSE, options) ? true : false;
		return result;
	}

	bool Handle::GetHeritable()
	{
		DWORD flags = 0;
		bool result = Kernel32::GetHandleInformation(m_h, &flags);
		if (result)
		{
			result = (flags & HANDLE_FLAG_INHERIT) != 0;
			return result;
		}
		else
		{
			continuable("Call to Kernel32::GetHandleInformaiton failed");
		}
		return false;
	}

	bool Handle::GetProtectFromClose()
	{
		DWORD flags = 0;
		bool result = Kernel32::GetHandleInformation(m_h, &flags);
		if (result)
		{
			result = (flags & HANDLE_FLAG_PROTECT_FROM_CLOSE) != 0;
			return result;
		}
		return false;
	}

	bool Handle::GetInformation(unsigned long* flags)
	{
		return Kernel32::GetHandleInformation(m_h, flags) ? true : false;
	}

	bool Handle::SetHeritable(bool inherit)
	{
		return Kernel32::SetHandleInformation(m_h, HANDLE_FLAG_INHERIT, inherit ? HANDLE_FLAG_INHERIT : 0) ? true : false;
	}

	void Handle::DontClose()
	{
		m_needsClose = false;
	}

	bool Handle::NeedsClose()
	{
		return m_needsClose;
	}

	bool Handle::SetProtectFromClose(bool close)
	{
		m_needsClose = !close;
		return Kernel32::SetHandleInformation(m_h, HANDLE_FLAG_PROTECT_FROM_CLOSE, close ? HANDLE_FLAG_PROTECT_FROM_CLOSE : 0) ? true : false;
	}

	bool Handle::SetInformation(unsigned long mask, unsigned long flags)
	{
		return Kernel32::SetHandleInformation(m_h, mask, flags) ? true : false;
	}

	void Handle::Close()
	{
		if (m_h && m_h != Invalid_Handle_Value && m_needsClose)
		{
			Kernel32::CloseHandle(m_h);
		}
		m_h = nullptr;
	}

	bool Handle::IsNull() const
	{
		return m_h == nullptr;
	}

	bool Handle::IsInvalid() const
	{
		return m_h == Invalid_Handle_Value;
	}

	void Handle::Drop()
	{
		m_h = nullptr;
	}

	void* Handle::Use() const
	{
		return m_h;
	}

}}}//qor::platform::win
