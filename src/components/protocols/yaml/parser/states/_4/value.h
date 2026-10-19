// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_VALUE
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_VALUE

#include "../../tokens.h"

namespace qor { namespace protocols { namespace yaml {

    //value = false / null / true / mapping / sequence / string
    class qor_pp_module_interface(QOR_YAML) value final : public AnyOneOf
    {
    public:
        value(Parser* parser);
        virtual ~value() noexcept;

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_VALUE