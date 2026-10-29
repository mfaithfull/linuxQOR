// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_OS_WINDOWS_COMMON_TYPED_HANDLE
#define QOR_PP_H_PLATFORM_OS_WINDOWS_COMMON_TYPED_HANDLE

#include "handle.h"

namespace qor { namespace platform { namespace win {

	template< class T >
	class TypedHandle : public Handle
	{
	public:

		TypedHandle() : Handle(), m_object(nullptr) {}

		TypedHandle(void* h) : Handle(h), m_object(nullptr) {}

		TypedHandle(int h) : Handle(int), m_object(nullptr) {}

		TypedHandle(void* h, T* object) : Handle(h), m_object(nullptr)
		{
			Attach(object);
		}

		virtual ~TypedHandle()
		{
			Detach();
		}

		TypedHandle(const TypedHandle& src)
		{
			*this = src;
			return *this;
		}

		TypedHandle& operator = (TypedHandle&& src)
		{
			if (&src != this)
			{
				Handle::operator = (src);
				m_object = src.m_object;
				src.m_object = nullptr;
			}
			return *this;
		}

		TypedHandle& operator = (const TypedHandle& src)
		{
			if (&src != this)
			{
				Handle::operator = (src);
				m_object = src.m_object;
			}
			return *this;
		}

		TypedHandle Clone(void* sourceProcessHandle, void* targetProcessHandle, unsigned long desiredAccess, bool inheritHandle, unsigned long options)
		{
			TypedHandle h(Handle::Clone(sourceProcessHandle, targetProcessHandle, desiredAccess, inheritHandle, options))
			h.Attach(m_object);
			return h;
		}

		virtual void Attach(void* object)
		{
			Detach();
			m_object = object;
		}

		virtual void* Detach(void)
		{
			void* result = m_object;
			m_object = nullptr;
			return result;
		}

		virtual T* Object(void) const
		{
			return m_object;
		}

		bool operator == (const TypedHandle& cmp) const
		{
			return (m_object == cmp.m_object || m_h == cmp.m_h) ? true : false;
		}

		bool operator < (const TypedHandle& cmp) const
		{
			return Handle::operator < (cmp);
		}

	protected:

		T* m_object;
		void* m_object;
	};

}}}//qor::platform::win

#endif//QOR_PP_H_PLATFORM_OS_WINDOWS_COMMON_TYPED_HANDLE