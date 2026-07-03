// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_IFUNCTIONCONTEXT
#define QOR_PP_H_IFUNCTIONCONTEXT

#include "src/qor/essentials/objectcontext/anyobject.h"

namespace qor {

#ifndef QOR_PP_H_ICALLCONTEXT
    class ICallContext;
#endif

    class IFunctionContext
    {
    public:

        virtual unsigned int Lock() = 0;
        virtual unsigned int Unlock() = 0;
        virtual bool Locked() const = 0;
        virtual ICallContext* GetCallContext() = 0;
        virtual IFunctionContext* GetParent() const = 0;
        virtual void SetParent(IFunctionContext* parent) = 0;
        virtual unsigned int TraceDepth() = 0;
        virtual const char* File() const = 0;
        virtual const char* Name() const = 0;
        virtual unsigned int Line() const = 0;
        virtual AnyObject TypedAny() const = 0;
        virtual const char* Module() const = 0;

    };
}//qor

#endif//QOR_PP_H_IFUNCTIONCONTEXT
