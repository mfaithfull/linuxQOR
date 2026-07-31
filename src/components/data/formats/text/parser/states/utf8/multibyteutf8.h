// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_TEXT_PARSER_MULTIBYTEUTF8CHAR
#define QOR_PP_H_DATA_TEXT_PARSER_MULTIBYTEUTF8CHAR

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace text {
    
    class qor_pp_module_interface(QOR_TEXTFORMAT) MultiByteUTF8Char : public ParserState
    {
    public: 
        MultiByteUTF8Char(Parser* parser);
        virtual ~MultiByteUTF8Char();

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}}//qor::data::parser::text

namespace qor{
    qor_pp_declare_source_of(data::parser::text::MultiByteUTF8Char, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::text::MultiByteUTF8Char>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_TEXT_PARSER_MULTIBYTEUTF8CHAR
