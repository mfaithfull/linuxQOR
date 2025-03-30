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

/*
	Copyright (C) 2017 by Sergey A Kryukov: derived work
	http://www.SAKryukov.org
	http://www.codeproject.com/Members/SAKryukov

	Based on original work by Sergey Ryazanov:
	"The Impossibly Fast C++ Delegates", 18 Jul 2005
	https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates

	MIT license:
	http://en.wikipedia.org/wiki/MIT_License

	Original publication: https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed

*/

#ifndef QOR_PP_H_DELEGATE
#define QOR_PP_H_DELEGATE

#include "delegateholder.h"

namespace qor {

    template< typename T > class Delegate;
    template< typename T > class MulticastDelegate;

    template< typename ret_t, typename ...params_t >
    class Delegate< ret_t(params_t...) > final : private DelegateHolder< ret_t(params_t...) >
    {
    public:

        Delegate() = default;
        
        bool IsNull() const
        {
            return m_Invocation.stub == nullptr;
        }

        bool operator == (void* ptr) const
        {
            return (ptr == nullptr) && this->IsNull();
        }

        bool operator != (void* ptr) const
        {
            return (ptr != nullptr) || (!this->IsNull());
        }

        Delegate(const Delegate& src)
        {
            src.m_Invocation.Clone(m_Invocation);
        }

        template< typename lambda_t >
        Delegate( const lambda_t& lambda )
        {
            Assign( (void*)(&lambda), LambdaStub<lambda_t> );
        }

        Delegate& operator = (const Delegate& src)
        {
            src.m_Invocation.Clone(m_Invocation);
            return *this;
        }

        template< typename lambda_t >
        Delegate& operator = (const lambda_t& src)
        {
            Assign( (void*)(&src), LambdaStub< lambda_t > );
            return *this;
        }

        bool operator == (const Delegate& Another) const
        {
            return m_Invocation == Another.m_Invocation;
        }

        bool operator != (const Delegate& Another ) const
        {
            return m_Invocation != Another.m_Invocation;
        }

        bool operator == (const MulticastDelegate< ret_t(params_t...)>& Another) const
        {
            return Another.m_Invocation == m_Invocation;
        }

        bool operator != (const MulticastDelegate< ret_t(params_t...)>& Another ) const
        {
            return Another != (*this);
        }

        template <class T, ret_t(T::*TMethod)(params_t...)>
        static Delegate Create(T* Instance)
        {
            return Delegate(Instance, MethodStub<T, TMethod>);
        }

        template <class T, ret_t(T::*TMethod)(params_t...) const>
        static Delegate Create(T const* Instance)
        {
            return Delegate(const_cast<T*>(Instance), ConstMethodStub<T, TMethod> );
        }

        template <ret_t(*TMethod)(params_t...)>
        static Delegate Create()
        {
            return Delegate(nullptr, FunctionStub<TMethod>);
        }

        template< typename lambda_t >
        static Delegate Create( const lambda_t& instance )
        {
            return Delegate((void*)(&instance), LambdaStub<lambda_t>);
        }

        ret_t operator()(params_t... arg) const
        {
            return (*m_Invocation.stub)(m_Invocation.object, arg...);
        }

    private:

        Delegate(void* anObject, typename DelegateHolder< ret_t(params_t...) >::stub_type aStub )
        {
            m_Invocation.object = anObject;
            m_Invocation.stub = aStub;
        }

        void Assign(void* anObject, typename DelegateHolder< ret_t(params_t...)>::stub_type aStub )
        {
            this->m_Invocation.object = anObject;
            this->m_Invocation.stub = aStub;
        }

        template <class T, ret_t(T::*TMethod)(params_t...)>
        static ret_t MethodStub(void* this_ptr, params_t... params) 
        {
            T* p = static_cast<T*>(this_ptr);
            return (p->*TMethod)(params...);
        }

        template <class T, ret_t(T::*TMethod)(params_t...) const>
        static ret_t ConstMethodStub(void* this_ptr, params_t... params) 
        {
            T* const p = static_cast<T*>(this_ptr);
            return (p->*TMethod)(params...);
        }

        template <ret_t(*TMethod)(params_t...)>
        static ret_t FunctionStub(void* this_ptr, params_t... params) 
        {
            return (TMethod)(params...);
        }

        template <typename lambda_t>
        static ret_t LambdaStub(void* this_ptr, params_t... arg)
        {
            lambda_t* p = static_cast<lambda_t*>(this_ptr);
            return (p->operator())(arg...);
        }

        friend class MulticastDelegate< ret_t(params_t...) >;
		typename DelegateHolder< ret_t(params_t...) >::InvocationElement m_Invocation;

    };

}//qor

#endif//QOR_PP_H_DELEGAT
