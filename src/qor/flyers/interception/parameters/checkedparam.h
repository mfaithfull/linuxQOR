// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_INTERCEPTION_CHECKED_PARAMETER
#define QOR_PP_H_INTERCEPTION_CHECKED_PARAMETER

#include "paramregister.h"
#include "checks/nocheck.h"

namespace qor {

    template< class T, class Check = NoCheck >
    class CheckedParam : public ParamRegister
    {
    public:

        typedef CheckedParam< T, Check > type;

        CheckedParam(T param) : ParamRegister(&m_Param), m_Param(param)
        {
            Check::Check(m_Param);
        }

        operator T()
        {
            return m_Param;
        }

        T& operator &()
        {
            return m_Param;
        }

        T* operator ->()
        {
            return &m_Param;
        }

    protected:

        T m_Param;
    };

}//qor

#endif//QOR_PP_H_INTERCEPTION_CHECKED_PARAMETER
//Review 2027/07/07