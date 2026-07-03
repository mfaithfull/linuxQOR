// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ICALLCONTEXT
#define QOR_PP_H_ICALLCONTEXT

namespace qor {

#ifndef QOR_PP_H_IFUNCTIONCONTEXT
    class IFunctionContext;
#endif

    class ICallContext
    {
    public:

        virtual void CallMade(IFunctionContext*) = 0;
        virtual void CallCompleted() = 0;
        virtual void OnReturnAssignment() = 0;
        virtual void OnReturn() = 0;

    };
}//qor

#endif//QOR_PP_H_ICALLCONTEXT
