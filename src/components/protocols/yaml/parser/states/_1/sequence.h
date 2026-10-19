// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_SEQUENCE
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_SEQUENCE

#include "../../tokens.h"

namespace qor { namespace protocols { namespace yaml {

    //sequence = begin-sequence [ value *( value-separator value ) ] end-sequence
    class qor_pp_module_interface(QOR_YAML) sequence final : public Sequence
    {
    public:
        sequence(Parser* parser);
        virtual ~sequence() noexcept;

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    private:
        Parser* m_parser;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_SEQUENCE