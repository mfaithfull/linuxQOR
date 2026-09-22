// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_IFUNCTIONCONTEXT
#define QOR_PP_H_IFUNCTIONCONTEXT

#include "src/qor/essentials/objectcontext/anyobject.h"

//Declare an abstract interface for function contexts

namespace qor
{

#ifndef QOR_PP_H_ICALLCONTEXT
    class ICallContext;
#endif

    class IFunctionContext
    {
    public:

        virtual unsigned int Lock() noexcept = 0;
        virtual unsigned int Unlock() noexcept = 0;
        virtual bool Locked() const noexcept = 0;
        virtual ICallContext* GetCallContext() noexcept = 0;
        virtual IFunctionContext* GetParent() const = 0;
        virtual void SetParent(IFunctionContext* parent) = 0;
        virtual unsigned int TraceDepth() const noexcept = 0;
        virtual const char* File() const noexcept = 0;
        virtual const char* Name() const noexcept = 0;
        virtual unsigned int Line() const noexcept = 0;
        virtual unsigned int Column() const noexcept = 0;
        virtual AnyObject TypedAny() const = 0;
        virtual const char* Module() const = 0;

    };
}//qor

#endif//QOR_PP_H_IFUNCTIONCONTEXT
