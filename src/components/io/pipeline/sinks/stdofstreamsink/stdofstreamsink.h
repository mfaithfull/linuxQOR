// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STDOFSTREAM
#define QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STDOFSTREAM

#include "src/framework/data/pipeline/iosink.h"
#include "src/components/io/pipeline/connectors/stdfstreamconnector/stdofstreamconnector.h"

namespace qor{ namespace io{ namespace components{ 

    class qor_pp_module_interface(QOR_STDOFSTREAMSINK) stdofstreamSink : public pipeline::iosink<stdofstreamConnector>
    {
    public:

        stdofstreamSink();
        virtual ~stdofstreamSink();
        virtual size_t WriteBytes(byte* data, size_t bytesToWrite);
    };

}}}//qor::io::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STDOFSTREAM
