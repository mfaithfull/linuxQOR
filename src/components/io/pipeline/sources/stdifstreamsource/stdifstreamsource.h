// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STDIFSTREAM
#define QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STDIFSTREAM

#include "src/framework/data/pipeline/iosource.h"
#include "src/components/io/pipeline/connectors/stdfstreamconnector/stdifstreamconnector.h"

namespace qor{ namespace io{ namespace components{ 

    class qor_pp_module_interface(QOR_STDIFSTREAMSOURCE) stdifstreamSource : public pipeline::iosource< stdifstreamConnector >
    {
    public:

        stdifstreamSource();
        virtual ~stdifstreamSource();

    protected:

        virtual size_t ReadBytes(byte* space, size_t bytesToRead);
    };

}}}//qor::io::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STDIFSTREAM
