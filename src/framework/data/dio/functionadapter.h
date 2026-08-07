// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_FUNCTIONADAPTER
#define QOR_PP_H_FRAMEWORK_DIO_FUNCTIONADAPTER

#include "src/qor/function/tef.h"
#include "src/qor/function/make_function.h"

namespace qor{ namespace dio{
    
    template<typename Ret, typename... Args>
    struct FunctionAdapter
    {
        FunctionAdapter(tef::Function<Ret(Args...)> f) 
        {
            m_ = f;
        }
                
        void operator()(Args&&... args)
        {
            return m_(args...);
        }

    private:

        tef::Function<Ret(Args...)> m_;
    };

    template<class T, typename Ret, typename... Args>
    struct MemberFunctionAdapter
    {        
        MemberFunctionAdapter(T& t, tef::MemberFunction<Ret(T::*)(Args...)> f) : m_{t}, m_f{f}
        { }

        Ret operator()(Args... args)
        {
            return m_f(m_, args...);
        }

    private:
        T& m_;
        tef::MemberFunction<Ret(T::*)(Args...)> m_f;
    };

    template<class T, typename Ret>
    struct MemberFunctionAdapter<T, Ret, void>
    {        
        MemberFunctionAdapter(T& t, tef::MemberFunction<Ret(T::*)(void)> f) : m_{t}, m_f{f}
        { }

        Ret operator()()
        {
            return m_f(m_);
        }

    private:
        T& m_;
        tef::MemberFunction<Ret(T::*)()> m_f;
    };

}}//qor:::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_FUNCTIONADAPTER

