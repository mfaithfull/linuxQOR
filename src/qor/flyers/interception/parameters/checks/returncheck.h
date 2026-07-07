// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_INTERCEPTION_PARAMETER_CHECKS_RETURNCHECK
#define QOR_PP_H_INTERCEPTION_PARAMETER_CHECKS_RETURNCHECK

namespace qor {

    template< typename TValue >
    class ReturnCheckBase
    {
    public:

        ReturnCheckBase() = default;
        virtual ~ReturnCheckBase() = default;

        virtual bool Test(TValue& /*value*/)
        {
            return false;
        }

        virtual void DefaultInit(TValue& /*value*/){ }

        static void Check(TValue& value, ReturnCheckBase* pInstance)
        {
            if (!pInstance->Test(value))
            {
                //TODO: maybe raise an issue
            }
        }

        static void Init(TValue& value, ReturnCheckBase* pInstance)
        {
            pInstance->DefaultInit(value);
        }
    };

    template< typename TValue >
    class NoReturnCheck : public ReturnCheckBase< TValue >
    {
    public:

        virtual bool Test(TValue& /*value*/)
        {
            return true;
        }
    };

}//qor

#endif//QOR_PP_H_INTERCEPTION_PARAMETER_CHECKS_RETURNCHECK
//Review 2027/07/07