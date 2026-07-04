// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_INTERCEPTION_PARAMETER_REGISTER
#define QOR_PP_H_INTERCEPTION_PARAMETER_REGISTER

#include "src/platform/compiler/compiler.h"
#include "src/qor/flyers/interception/functioncontextlock.h"

namespace qor {

    struct qor_pp_module_interface(QOR_INTERCEPTION) ParamRegister
    {
        template< typename T >ParamRegister(T* pParam)
        {
            FunctionContextLock fcl;
            dynamic_cast<CallContext*>(fcl.CallContextPtr())->Register(*pParam);
        }
    };

}//qor

#endif//QOR_PP_H_INTERCEPTION_PARAMETER_REGISTER
