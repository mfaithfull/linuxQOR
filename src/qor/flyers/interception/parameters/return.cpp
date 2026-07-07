// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "return.h"
#include "src/qor/essentials/current/currentthread.h"

namespace qor {

    void ReturnRegister::OnAssignment()
    {
        FunctionContextLock fcl;
        fcl.ParentCallContextPtr()->OnReturnAssignment();
    }

    void ReturnRegister::OnReturn()
    {
        FunctionContextLock fcl;
        fcl.ParentCallContextPtr()->OnReturn();
    }

}//qor
//Review 2027/07/07