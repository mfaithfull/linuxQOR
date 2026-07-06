// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STDOUT
#define QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STDOUT

#include "src/framework/data/pipeline/sink.h"

namespace qor{ namespace io{ namespace components{ 

    class qor_pp_module_interface(QOR_STDOUTSINK) StdOutSink : public pipeline::Sink
    {
    public:

        StdOutSink();
        virtual ~StdOutSink();

        //virtual bool Write(size_t& unitsWritten, size_t unitstoWrite = 1 );

    private:
        
        //bool Pull(size_t& unitsWritten, size_t unitstoWrite);
        bool Push(size_t& unitsWritten, size_t unitstoWrite);

    };

}}}//qor::io::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STDOUT
