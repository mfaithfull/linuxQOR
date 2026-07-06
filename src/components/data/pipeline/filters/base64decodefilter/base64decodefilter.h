// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_BASE64DECODEFILTER
#define QOR_PP_H_PIPELINE_BASE64DECODEFILTER

#include "src/framework/data/pipeline/filter.h"

namespace qor{ namespace pipeline{ namespace components{

    class qor_pp_module_interface(QOR_BASE64DECODE) Base64DecodeFilter : public Filter
    {
    public:

        Base64DecodeFilter();
        virtual ~Base64DecodeFilter();

        virtual bool ReadFilter(size_t& unitsProcessed, size_t unitsToProcess);
        virtual bool WriteFilter(size_t& unitsProcessed, size_t unitsToProcess);
        virtual bool Decode(size_t& unitsProcessed, size_t unitsToProcess);
                
    };

}}}//qor::pipeline::components

#endif//QOR_PP_H_PIPELINE_BASE64DECODEFILTER
