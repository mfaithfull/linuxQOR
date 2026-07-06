// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STDIN
#define QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STDIN

#include "src/framework/data/pipeline/source.h"

namespace qor{ namespace io{ namespace components{ 

    class qor_pp_module_interface(QOR_STDINSOURCE) StdInSource : public pipeline::Source
    {
    public:

        StdInSource();
        virtual ~StdInSource();

        bool LineMode(bool newMode = true);
        string_t ReadLine(size_t& unitsRead, size_t unitsToRead);
        virtual bool Read(size_t& numberOfUnitsRead, size_t numberOfUnitsToRead = 1);

    private:

        bool m_lineMode;
        bool Pull(size_t& unitsRead, size_t unitsToRead);
        bool Push(size_t& unitsWritten, size_t unitsToWrite);
    };

}}}//qor::io::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STDIN
