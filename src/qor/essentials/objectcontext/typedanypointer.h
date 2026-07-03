// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TYPEDANYPOINTER
#define QOR_PP_H_TYPEDANYPOINTER

#include <stdexcept>

#include "src/qor/essentials/objectcontext/unsafeanypointer.h"

namespace qor{

    template< class T >
    class TypedAnyPointer : public UnsafeAnyPointer
    {
    public:

        TypedAnyPointer() : UnsafeAnyPointer(){}
        
        TypedAnyPointer( const T* pt ) : UnsafeAnyPointer( const_cast<void*>( reinterpret_cast< const void*>(pt))){}

        TypedAnyPointer(const TypedAnyPointer& src) : UnsafeAnyPointer(reinterpret_cast<void*>(src.m_p)){}

        TypedAnyPointer& operator = (const TypedAnyPointer& src)
        { 
            UnsafeAnyPointer::operator=(src);
            return *this;
        }

        virtual ~TypedAnyPointer() = default;

        operator T*() const
        {
            return reinterpret_cast<T*>(m_p);
        }

        T* operator -> () const
        {
            if( IsNull() )
            {
                throw std::logic_error("Null reference exception: TypedAnyPointer must point to an object to be used.");
            }
            return reinterpret_cast<T*>(m_p);
        }
    };

}//qor

#endif//QOR_PP_H_TYPEDANYPOINTER
