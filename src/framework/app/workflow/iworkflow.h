// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WORKFLOW_INTERFFACE
#define QOR_PP_H_WORKFLOW_INTERFFACE

#include "../application/irunable.h"

namespace qor{ namespace workflow{

    //Base abstract interface for all workflows
    class IWorkflow : public app::IRunable
    {
    public:

        IWorkflow() = default;
        virtual ~IWorkflow() = default;

        virtual int Run() = 0;
        virtual bool IsComplete() const = 0;

    };

}}//qor::workflow

#endif//QOR_PP_H_WORKFLOW_INTERFFACE
