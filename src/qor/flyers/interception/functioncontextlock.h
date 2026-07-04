// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_INTERCEPTION_FC_LOCK
#define QOR_PP_H_INTERCEPTION_FC_LOCK

#include "functioncontext.h"

//Protect the AOP mechanisms from operatiing on themselves by locking the Function Context at the entry point
//This is not a thread synchronisation lokc but a form of recursion limit within one thread

namespace qor {

    class qor_pp_module_interface(QOR_INTERCEPTION) FunctionContextLock final
    {
    public:

        FunctionContextLock();
        ~FunctionContextLock();

        CallContext* CallContextPtr();
        CallContext* ParentCallContextPtr();
        unsigned long Level();

    private:

        FunctionContextLock(const FunctionContextLock&);
        FunctionContextLock& operator=(const FunctionContextLock&);

        CallContext* m_CallContext{nullptr};
        CallContext* m_ParentCallContext{nullptr};
        IFunctionContext* m_FunctionContext{nullptr};
        unsigned long m_Level{(unsigned long)(-1)};
    };

}//qor

#endif//QOR_PP_H_INTERCEPTION_FC_LOCK
