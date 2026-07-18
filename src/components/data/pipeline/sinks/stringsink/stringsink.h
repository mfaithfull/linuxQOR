// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STRING
#define QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STRING

#include <string>
#include "src/framework/data/pipeline/sink.h"

namespace qor{ namespace pipeline{ namespace components{ 

    class qor_pp_module_interface(QOR_STRINGSINK) StringSink : public Sink
    {
    public:

        StringSink();
        virtual ~StringSink();

        std::string GetData();

    protected:

        virtual size_t WriteBytes(byte* data, size_t bytesToWrite);

        std::string m_data;
    };

}}}//qor::pipeline::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STRING
