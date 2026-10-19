// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_STRING
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_STRING

#include "../../tokens.h"
#include "quotation_mark.h"
#include "character.h"
#include "src/qor/essentials/text/codepage/codepages/utf8.h"

namespace qor { namespace protocols { namespace yaml {

    //string = quotation_mark *character quotation_mark
    class string final : public Sequence_t< quotation_mark, Sequence_t< ZeroOrMore_t< character >, quotation_mark > >
    {
    public:
        string(Parser* parser);
        virtual ~string() noexcept;
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    private:
        quotation_mark m_qopen;
        quotation_mark m_qclose;
        character m_char;
        ZeroOrMore_t< character > m_body;
        Sequence_t< ZeroOrMore_t< character >, quotation_mark > m_tail;
        static UTF8CodePage s_codePage;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_STRING