// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_UNSAFEANYPOINTER
#define QOR_PP_H_UNSAFEANYPOINTER

namespace qor {

    //Always a weak pointer
    //Wraps a void pointer as an object and provides an unsafe cast to any type
    //Don't use it unless you know 100% what the real type is
    class UnsafeAnyPointer
    {
    public:
    
        UnsafeAnyPointer(void* p = nullptr) : m_p(p) {}

        UnsafeAnyPointer(const UnsafeAnyPointer& src)
        {
            *this = src;
        }

        UnsafeAnyPointer(UnsafeAnyPointer&& src)
        {
            m_p = src.m_p;
        }

        virtual ~UnsafeAnyPointer()
        {
            m_p = nullptr;
        }

        UnsafeAnyPointer& operator = (const UnsafeAnyPointer& src)
        {
            m_p = src.m_p;
            return *this;
        }

        template< class T > operator T*() const
        {
            return reinterpret_cast<T*>(m_p);
        }

        bool IsNull() const
        {
            return m_p == nullptr ? true : false;
        }

    protected:

        void* m_p{nullptr};
    };
}//qor

#endif//QOR_PP_H_UNSAFEANYPOINTER
