// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STRING
#define QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STRING

#include <string>
#include "src/framework/data/pipeline/iosource.h"

namespace qor{ namespace pipeline { namespace components{ 

    class qor_pp_module_interface(QOR_STRINGSOURCE) StringSource : public pipeline::Source
    {

    public:

        StringSource();
        virtual ~StringSource();
        void SetData(const std::string& data);

        //virtual bool Read(size_t& numberOfUnitsRead, size_t numberOfUnitsToRead = 1);

    protected:

        virtual size_t ReadBytes(byte* space, size_t bytesToRead);
        //bool Pull(size_t& unitsRead, size_t unitsToRead);
        //bool Push(size_t& unitsRead, size_t unitsToRead);

        std::string m_data;
        std::string::iterator m_it;
    };

}}}//qor::pipeline::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STRING
