// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_BASE64ENCODEFILTER
#define QOR_PP_H_PIPELINE_BASE64ENCODEFILTER

#include "src/framework/data/pipeline/inlinefilter.h"

namespace qor{ namespace pipeline{ namespace components{

    class qor_pp_module_interface_gcc(QOR_BASE64ENCODE) Base64EncodeFilter : public InlineFilter<byte>
    {
    public:

        qor_pp_module_interface(QOR_BASE64ENCODE) Base64EncodeFilter();
        qor_pp_module_interface(QOR_BASE64ENCODE) Base64EncodeFilter(size_t itemCount);
        qor_pp_module_interface(QOR_BASE64ENCODE) virtual ~Base64EncodeFilter();
        qor_pp_module_interface(QOR_BASE64ENCODE) virtual void Filter(qor::byte* space, qor::byte* data, size_t& itemCount, size_t& writeCount);

    };

}}}//qor::pipeline::components

#endif//QOR_PP_H_PIPELINE_BASE64ENCODEFILTER
