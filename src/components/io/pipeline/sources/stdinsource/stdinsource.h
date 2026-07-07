// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STDIN
#define QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STDIN

#include "src/framework/data/pipeline/source.h"

namespace qor{ namespace io{ namespace components{ 

    class qor_pp_module_interface_gcc(QOR_STDINSOURCE) StdInSource : public pipeline::Source
    {
    public:

        qor_pp_module_interface(QOR_STDINSOURCE) StdInSource();
        qor_pp_module_interface(QOR_STDINSOURCE) StdInSource(const StdInSource&);
        qor_pp_module_interface(QOR_STDINSOURCE) StdInSource& operator = (const StdInSource&);
        qor_pp_module_interface(QOR_STDINSOURCE) virtual ~StdInSource();
        qor_pp_module_interface(QOR_STDINSOURCE) bool LineMode(bool newMode = true);
        qor_pp_module_interface(QOR_STDINSOURCE) string_t ReadLine(size_t& unitsRead, size_t unitsToRead);

    private:

        bool m_lineMode;
        bool Pull(size_t& unitsRead, size_t unitsToRead);        
    };

}}}//qor::io::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STDIN
