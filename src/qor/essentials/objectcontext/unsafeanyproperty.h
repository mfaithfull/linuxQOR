// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0


#ifndef QOR_PP_H_UNSAFEANYPROPERTY
#define QOR_PP_H_UNSAFEANYPROPERTY

#include <string.h>
#include <typeinfo>

#include "src/platform/compiler/compiler.h"

namespace qor {

    //Wraps a byte pointer as an object and provides an unsafe cast to any type
    //Don't use it unless you know 100% what the real type is
    class UnsafeAnyProperty
    {
    public:

        UnsafeAnyProperty() : m_internalCopy(nullptr) {}

        UnsafeAnyProperty(size_t size)
        {
            m_internalCopy = new byte[size];
            memset(m_internalCopy, 0, size);
        }

        virtual ~UnsafeAnyProperty()
        {
            delete m_internalCopy;
        }

        bool IsNull() const
        {
            return m_internalCopy == nullptr ? true : false;
        }

        void* Ptr() const
        {
            return m_internalCopy;
        }

        virtual size_t Size() const
        {
            return 0;
        }

        virtual UnsafeAnyProperty* Clone() const
        {
            return nullptr;
        }

        virtual const char* TypeId() const
        {
            return typeid(void).name();
        }

        template<class T>
        T* GetAs() const
        {
            if (sizeof(T) == Size() && typeid(T).name() == TypeId())
            {
                return reinterpret_cast<T*> (m_internalCopy);
            }
            return nullptr;
        }

    protected:

        byte* m_internalCopy;
        
    };
}//qor

#endif//QOR_PP_H_UNSAFEANYPOINTER
