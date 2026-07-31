// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_TEXT_PARSER_TEXT
#define QOR_PP_H_DATA_TEXT_PARSER_TEXT

#include "../tokens.h"

namespace qor { namespace data { namespace parser { namespace text {

    //text = [ bom ] *( character )
    class qor_pp_module_interface(QOR_TEXTDATA) text : public Sequence
    {
    public: 
        text(Parser* parser);
        virtual ~text();

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    private:

        ref_of<ParserState>::type CharacterType(Parser* parser);
    };

}}}}//qor::data::parser::text

namespace qor{
    qor_pp_declare_source_of(data::parser::text::text, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::text::text>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_TEXT_PARSER_TEXT
