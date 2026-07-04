// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FLYERREF
#define QOR_PP_H_FLYERREF

#include <atomic>
#include "src/framework/parallel/sync/sync.h"
#include "src/qor/memory/instance/instance.h"
#include "src/qor/memory/memory.h"

namespace qor{

    //A weak reference to an object that must already exist down the stack from this reference
	//FlyerRef is inherently single threaded. It always referes to an object local to the same 
    //thread where it is created. Do not pass across threads.
	template< class T > 
	class FlyerRef final
	{
	public:

        FlyerRef() : m_p(nullptr){ }

        FlyerRef(const T* t) : m_p(const_cast<T*>(t)){ }

        FlyerRef(const FlyerRef< T >& src)
		{
            m_p = src.m_p;
		}

		FlyerRef(FlyerRef< T >&& Src) noexcept
		{
			m_p = Src.m_p;
		}

        ~FlyerRef() = default;

		FlyerRef& operator = (const FlyerRef< T >& src)
		{
			if (this != &src)
			{
				m_p = src.m_p;
			}
			return *this;
		}

        FlyerRef& operator = (FlyerRef< T >&& src)
		{
			if (this != &src)
			{
				m_p = src.m_p;
			}
			return *this;
		}

		//Two references are equal if they refer to the same object
		bool operator == (const FlyerRef< T >& cmp) const
		{
			return (m_p == cmp.m_p);
		}

		operator T* (void) const
		{
			return const_cast<T*>(m_p);
		}

		T& operator() (void) const
		{
			if (m_p == nullptr)
			{
				//TODO: Raise through qor_reporting instead
				throw new std::logic_error("Null reference exception: A reference must refer to an object in order to be used.");
			}
			return const_cast<T&>(*m_p);
		}

		T* operator -> () const
		{
			if (m_p == nullptr)
			{
				//TODO: Raise through qor_reporting instead
				throw new std::logic_error("Null reference exception: A reference must refer to an object in order to be used.");
			}
			return const_cast<T*>(m_p);
		}

		void Dispose(void){ }

		template< class TDerived >
		TDerived* As(void)
		{
			return dynamic_cast<TDerived*>(m_p);
		}

		template< class TDerived >
		FlyerRef< TDerived > AsRef(void)
		{
			if (dynamic_cast<TDerived*>(m_p) != nullptr)
			{
				return FlyerRef< TDerived >(reinterpret_cast<TDerived*>(m_p));
			}
			return FlyerRef< TDerived >(nullptr);
		}

		bool IsNull(void) const
		{
			return m_p == nullptr;
		}

		bool IsNotNull(void) const
		{
			return m_p != nullptr;
		}

		bool IsOwner(void) const
		{
			return false;
		}

		template< class TDerived >
		bool Configure()
		{
			return false;
		}

		template< class TDerived, typename... _p >
		bool Configure(_p... p1)
		{
			return false;
		}

	protected:

        const T* m_p;
	};

}//qor

#endif//QOR_PP_H_FLYERREF
