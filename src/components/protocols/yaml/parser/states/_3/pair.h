// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_PAIR
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_PAIR

#include "../../tokens.h"
#include "../_4/string.h"
#include "name_separator.h"
#include "../_4/value.h"

namespace qor { namespace protocols { namespace yaml {

    //pair = string name-separator value
    class qor_pp_module_interface(QOR_YAML) pair final : public Sequence_t< string, Sequence_t< name_separator, value > >
    {
    public:
        pair(Parser* parser);
        virtual ~pair() noexcept;

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    private:
        value m_v;
        name_separator m_ns;
        string m_str;
        Sequence_t< name_separator, value > m_tail;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_PAIR