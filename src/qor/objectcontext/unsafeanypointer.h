// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef QOR_PP_H_UNSAFEANYPOINTER
#define QOR_PP_H_UNSAFEANYPOINTER

namespace qor {

    //Wraps a void pointer as an object and provides an unsafe cast to any type
    //Don't use it unless you know 100% what the real type is
    class qor_pp_module_interface(QOR_OBJECTCONTEXT) UnsafeAnyPointer
    {
    public:
    
        UnsafeAnyPointer(const UnsafeAnyPointer& src)
        {
            *this = src;
        }

        UnsafeAnyPointer(void* p = nullptr) : m_p(p) {}

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

        void* m_p;
    };
}//qor

#endif//QOR_PP_H_UNSAFEANYPOINTER
