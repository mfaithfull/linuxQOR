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

#ifndef QOR_PP_H_TYPEDANYPOINTER
#define QOR_PP_H_TYPEDANYPOINTER

#include <stdexcept>

#include "src/qor/objectcontext/unsafeanypointer.h"

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
