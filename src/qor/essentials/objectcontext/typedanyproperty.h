// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TYPEDANYPROPERTY
#define QOR_PP_H_TYPEDANYPROPERTY

#include <stdexcept>

#include "src/qor/essentials/objectcontext/unsafeanyproperty.h"

namespace qor {

    template< class T >
    class TypedAnyProperty : public UnsafeAnyProperty
    {
    public:

        TypedAnyProperty() : UnsafeAnyProperty(sizeof(T)) {}

        TypedAnyProperty(const T& t) : UnsafeAnyProperty(sizeof(T)) 
        {
            new (Ptr())T(t);
        }

        TypedAnyProperty(const TypedAnyProperty& src) : UnsafeAnyProperty(sizeof(T)) 
        {
            new (Ptr())T(src.Get());          
        }

        TypedAnyProperty& operator = (const TypedAnyProperty& src)
        {
            return operator=(src.Get());
        }

        TypedAnyProperty& operator = (const T& src)
        {
            *(reinterpret_cast<T*>(m_internalCopy)) = src;
            return *this;
        }

        virtual ~TypedAnyProperty()
        {
            (reinterpret_cast<T*>(m_internalCopy))->~T();
        }

        operator T () const
        {
            return *(reinterpret_cast<T*>(m_internalCopy));
        }

        T Get() const
        {
            return *(reinterpret_cast<T*>(m_internalCopy));
        }

        virtual size_t Size() const
        {
            return sizeof(T);
        }

        virtual UnsafeAnyProperty* Clone() const
        {
            return new TypedAnyProperty<T>(*this);
        }

        virtual const char* TypeId() const
        {
            return typeid(T).name();
        }
    };

}//qor

#endif//QOR_PP_H_TYPEDANYPROPERTY
