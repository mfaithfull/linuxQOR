// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STDOUT
#define QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STDOUT

#include "src/framework/data/pipeline/sink.h"

namespace qor{ namespace io{ namespace components{ 

    class qor_pp_module_interface_gcc(QOR_STDOUTSINK) StdOutSink : public pipeline::Sink
    {
    public:

        qor_pp_module_interface(QOR_STDOUTSINK) StdOutSink();
        qor_pp_module_interface(QOR_STDOUTSINK) StdOutSink(const StdOutSink& src);
        qor_pp_module_interface(QOR_STDOUTSINK) StdOutSink& operator = (const StdOutSink& src);
        qor_pp_module_interface(QOR_STDOUTSINK) virtual ~StdOutSink();

    private:
        
        bool Push(size_t& unitsWritten, size_t unitstoWrite);

    };

}}}//qor::io::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STDOUT
