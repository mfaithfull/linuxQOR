// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_COPYFILTER
#define QOR_PP_H_PIPELINE_COPYFILTER

#include "filter.h"

namespace qor{ namespace pipeline{

    class qor_pp_module_interface(QOR_PIPELINE) CopyFilter : public Filter
    {
    public:

        virtual bool ReadFilter(size_t& unitsProcessed, size_t unitsToProcess);
        virtual bool WriteFilter(size_t& unitsProcessed, size_t unitsToProcess);
        virtual bool Process( byte* dest, byte* source, size_t sourceUnits, size_t sourceUnitSize, size_t destUnitSize);
        
    protected:

        bool Copy(size_t& unitsProcessed, size_t unitsToProcess);
        
    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_COPYFILTER
