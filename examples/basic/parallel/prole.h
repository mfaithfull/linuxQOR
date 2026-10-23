// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_PARALLEL_PROLE
#define QOR_PP_H_EXAMPLES_PARALLEL_PROLE

#include "src/framework/app/role/role.h"
#include "src/components/qor/logaggregator/logaggregator.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/defaultloghandler.h"
#include "src/framework/io/filesystem/filesystem.h"

class ParallelAppRole : public qor::app::Role
{
public:

    constexpr static const char* logTag = "parallel_app";

    virtual void Setup();
    virtual void Shutdown();

private:

    void SetupLogging(qor::DefaultLogHandler& logHandler, qor::components::LogAggregatorService::ref logAggregator);
};

#endif//QOR_PP_H_EXAMPLES_PARALLEL_PROLE