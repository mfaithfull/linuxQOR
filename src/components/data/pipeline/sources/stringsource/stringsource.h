// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STRING
#define QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STRING

#include <string>
#include "src/framework/data/pipeline/iosource.h"

namespace qor{ namespace pipeline { namespace components{ 

    class qor_pp_module_interface_gcc(QOR_STRINGSOURCE) StringSource : public pipeline::Source
    {

    public:

        qor_pp_module_interface(QOR_PIPELINE) StringSource();
        qor_pp_module_interface(QOR_PIPELINE) virtual ~StringSource();
        qor_pp_module_interface(QOR_PIPELINE) void SetData(const std::string& data);

    protected:

        qor_pp_module_interface(QOR_PIPELINE) virtual size_t ReadBytes(byte* space, size_t bytesToRead);

        std::string m_data;
        std::string::iterator m_it;
    };

}}}//qor::pipeline::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STRING
