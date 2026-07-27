// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SINKS_PIPE
#define QOR_PP_H_COMPONENTS_PIPELINE_SINKS_PIPE

#include "src/framework/data/pipeline/iosink.h"
#include "pipeconnector.h"

namespace qor{ namespace io{ namespace components{

    class qor_pp_module_interface_gcc(QOR_PIPECONNECTOR) PipeSink : public pipeline::iosink<PipeConnector>
    {
    public:

        qor_pp_module_interface(QOR_PIPECONNECTOR) PipeSink();
        qor_pp_module_interface(QOR_PIPECONNECTOR) virtual ~PipeSink();

    protected:

        virtual size_t WriteBytes(byte* data, size_t bytesToWrite);
    };

}}}//qor::io::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SINKS_PIPE
