// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_FILTER_UNICODE_TEXT
#define QOR_PP_H_PIPELINE_FILTER_UNICODE_TEXT

#include "src/framework/data/pipeline/inlineprocessor.h"

namespace qor{ namespace text{ namespace components {

    class qor_pp_module_interface_gcc(QOR_UTEXTFILTER) UnicodeTextFilter : public pipeline::InlineProcessor<uint32_t, byte>
    {
    public:

        qor_pp_module_interface(QOR_UTEXTFILTER) UnicodeTextFilter();        
        qor_pp_module_interface(QOR_UTEXTFILTER) virtual ~UnicodeTextFilter();
        qor_pp_module_interface(QOR_UTEXTFILTER) virtual void Process(size_t& itemCount) override;

    };

}}}//qor::text::components

#endif//QOR_PP_H_PIPELINE_FILTER_UNICODE_TEXT
