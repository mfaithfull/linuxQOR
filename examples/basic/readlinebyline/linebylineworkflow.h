// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_LINEBYLINE_WORKFLOW
#define QOR_PP_H_EXAMPLES_LINEBYLINE_WORKFLOW

#include "src/framework/parallel/thread/currentthread.h"
#include "src/qor/reference/newref.h"
#include "src/framework/app/workflow/workflow.h"

class LineByLineWorkflow : public qor::workflow::Workflow
{
public:

    LineByLineWorkflow();
    virtual ~LineByLineWorkflow() = default;    

private:

    qor::ref_of<qor::workflow::State>::type state0;
};

#endif//#ifndef QOR_PP_H_EXAMPLES_LINEBYLINE_WORKFLOW
