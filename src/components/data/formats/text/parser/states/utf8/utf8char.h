// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_TEXT_PARSER_UTF8CHAR
#define QOR_PP_H_DATA_TEXT_PARSER_UTF8CHAR

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace text {
    
    class qor_pp_module_interface(QOR_TEXTFORMAT) utf8char : public OneOfARange//public AnyOneOf
    {
    public: 
        utf8char(Parser* parser);
        virtual ~utf8char();

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    private:

        Parser* m_parser;
    };

}}}}//qor::data::parser::text

namespace qor{
    qor_pp_declare_source_of(data::parser::text::utf8char, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::text::utf8char>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_TEXT_PARSER_UTF8CHAR
