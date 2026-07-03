// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ANYOBJECT
#define QOR_PP_H_ANYOBJECT

#include <string.h>
#include "src/qor/essentials/objectcontext/typedanypointer.h"
#include "src/platform/compiler/compiler.h"

namespace qor{

	class AnyObject;

	constexpr const int size_ptr_obj = sizeof(TypedAnyPointer< AnyObject >);

    //A type erased container for a typed pointer to anything
    class AnyObject
    {
    public:

        template< typename T > AnyObject(const T* t)
        {
            m_p = new (&m_backing)TypedAnyPointer< T >(t);
        }

		AnyObject() = default;
		
		AnyObject(const AnyObject& src)
		{
			*this = src;
		}

		AnyObject& operator = (const AnyObject& src)
		{
			if (src.m_p != nullptr)
			{
				memcpy(m_backing, src.m_backing, size_ptr_obj);				
				m_p = reinterpret_cast<UnsafeAnyPointer*>(&m_backing);
			}
			else
			{
				m_p = nullptr;
				Clear();
			}
			return *this;
		}

		virtual ~AnyObject()
		{
			if (m_p)
			{
				m_p->~UnsafeAnyPointer();
			}
		}

		inline bool IsNull() const
		{
			return (m_p == nullptr || m_p->IsNull());
		}

        template< typename T>
        operator T* () const
		{
			TypedAnyPointer< T >* op = dynamic_cast< TypedAnyPointer< T >* >(m_p);
			return op ? op->operator T *() : nullptr;
		}
		
		static AnyObject& EmptyObject()
		{
			static AnyObject _emptyObject;
			return _emptyObject;
		}

		const UnsafeAnyPointer* Ptr() const
		{
			return m_p;
		}

    protected:

		UnsafeAnyPointer* m_p{nullptr};

	private:

		unsigned char m_backing[size_ptr_obj]{0};

		void Clear()
		{
			memset(m_backing, 0, size_ptr_obj);
			m_p = nullptr;
		}

    };

}//qor

#endif//QOR_PP_H_ANYOBJECT
