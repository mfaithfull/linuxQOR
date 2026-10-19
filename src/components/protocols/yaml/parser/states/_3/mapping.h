// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_MAPPING
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_MAPPING

#include "../../tokens.h"

namespace qor { namespace protocols { namespace yaml {

    //mapping = begin-mapping [ pair *( value-separator pair ) ] end-mapping
    class qor_pp_module_interface(QOR_YAML) mapping final : public Sequence
    {
    public:
        mapping(Parser* parser);
        virtual ~mapping() noexcept;

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_MAPPING