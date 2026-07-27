// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_PIPE
#define QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_PIPE

#include "src/framework/data/pipeline/iosource.h"
#include "pipeconnector.h"

namespace qor{ namespace io{ namespace components{ 

    class qor_pp_module_interface_gcc(QOR_PIPECONNECTOR) PipeSource : public pipeline::iosource<PipeConnector>
    {    
    public:

        qor_pp_module_interface(QOR_PIPECONNECTOR) PipeSource();
        qor_pp_module_interface(QOR_PIPECONNECTOR) virtual ~PipeSource();

    protected:

        virtual size_t ReadBytes(byte* data, size_t bytesToRead);
       
    };

}}}//qor::io:components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_PIPE
