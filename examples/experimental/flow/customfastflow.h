// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_WORK_CUSTOMFASTFLOW
#define QOR_PP_H_EXAMPLES_WORK_CUSTOMFASTFLOW

#include "src/framework/app/workflow/fastflow.h"

class CustomFastflow : public qor::Fastflow //Workflows are always overrides of the base Workflow class.
{
public:

    CustomFastflow();
    virtual ~CustomFastflow() = default;    

private:

    qor::fastflow::Step::ref beggining;
    qor::fastflow::Step::ref middle;
    qor::fastflow::Step::ref end;
};

#endif//QOR_PP_H_EXAMPLES_WORK_CUSTOMFASTFLOW