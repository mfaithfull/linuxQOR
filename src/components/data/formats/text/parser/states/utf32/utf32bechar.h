// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_TEXT_PARSER_UTF32BECHAR
#define QOR_PP_H_DATA_TEXT_PARSER_UTF32BECHAR

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace text {
    
    class qor_pp_module_interface(QOR_TEXTFORMAT) utf32BEchar : public Sequence
    {
    public: 
        utf32BEchar(Parser* parser);
        virtual ~utf32BEchar();

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}}//qor::data::parser::text

#endif//QOR_PP_H_DATA_TEXT_PARSER_UTF32BECHAR
