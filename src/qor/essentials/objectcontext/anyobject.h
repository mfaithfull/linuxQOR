// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ANYOBJECT
#define QOR_PP_H_ANYOBJECT

#include "src/qor/essentials/objectcontext/typedanypointer.h"
#include "src/platform/compiler/compiler.h"

namespace qor{

    //A type erased container for a typed pointer to anything
    class qor_pp_module_interface(QOR_OBJECTCONTEXT) AnyObject
    {
    public:

        template< typename T > AnyObject(const T* t)
        {
            m_p = new (m_backing)TypedAnyPointer< T >(t);
        }

		AnyObject();
		AnyObject(const AnyObject& src);
		AnyObject& operator = (const AnyObject& src);
		virtual ~AnyObject();

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

		static AnyObject& NullObject(void);
		static AnyObject nullObject;

		const UnsafeAnyPointer* Ptr() const
		{
			return m_p;
		}

    protected:

		UnsafeAnyPointer* m_p;

	private:

		typedef TypedAnyPointer< AnyObject > ObjectContextBasePointer;
		byte m_backing[sizeof(ObjectContextBasePointer) + sizeof(double)];

		byte* Local_memcpy(byte* s1, const byte* s2, size_t n);
		void Clear(void);
    };

}//qor

#endif//QOR_PP_H_ANYOBJECT
