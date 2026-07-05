// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_INTERCEPTION_PARAMETERS_RETURN
#define QOR_PP_H_INTERCEPTION_PARAMETERS_RETURN

#include "src/qor/flyers/interception/functioncontextlock.h"
#include "src/qor/flyers/interception/parameters/checks/returncheck.h"

namespace qor {

    template< typename TValue > class NoReturnCheck;

    struct ReturnRegister
    {
    public:
        
        template< typename T >ReturnRegister(T* pReturn)
        {
            FunctionContextLock fcl;
            fcl.ParentCallContextPtr()->RegisterReturn(*pReturn);
        }

        qor_pp_module_interface(QOR_INTERCEPTION) void OnAssignment();
        qor_pp_module_interface(QOR_INTERCEPTION) void OnReturn();
    };

    template< class T, class Check = NoReturnCheck< T > >
    class CheckedReturn : public ReturnRegister
    {
    public:

        typedef CheckedReturn< T, Check > type;

        CheckedReturn() : ReturnRegister(&m_Param), m_bInitialised(false)
        {
            Check AutoCheck;
            Check::Init(m_Param, &AutoCheck);
        }

        CheckedReturn(T param) : ReturnRegister(&m_Param), m_Param(param), m_bInitialised(true)
        {
            Check AutoCheck;
            Check::Check(m_Param, &AutoCheck);
            OnAssignment();
        }
        
        CheckedReturn& operator = (T& value)
        {
            m_Param = value;
            m_bInitialised = true;
            Check AutoCheck;
            Check::Test(m_Param, &AutoCheck);
            OnAssignment();
            return *this;
        }

        operator T()
        {
            if (!m_bInitialised)
            {
                //TODO: maybe raise an issue
                //error::Error<CUninitialisedReturnWarning>("");
            }
            OnReturn();
            return m_Param;
        }

    protected:

        T m_Param;			//Instance of parameter type

    private:

        bool m_bInitialised;

    };

}//qor

#endif//QOR_PP_H_INTERCEPTION_PARAMETERS_RETURN
