// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_TEXT_PARSER_UTF16BECHAR
#define QOR_PP_H_DATA_TEXT_PARSER_UTF16BECHAR

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace text {
    
    class qor_pp_module_interface(QOR_TEXTFORMAT) utf16BEchar : public Sequence
    {
    public: 
        utf16BEchar(Parser* parser);
        virtual ~utf16BEchar();

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}}//qor::data::parser::text

#endif//QOR_PP_H_DATA_TEXT_PARSER_UTF16BECHAR
