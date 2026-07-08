// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_ESCAPESEQUENCE
#define QOR_PP_H_COMPONENTS_JSON_PARSER_ESCAPESEQUENCE

#include "../tokens.h"
#include "escape.h"
#include "unicodesequence.h"

namespace qor { namespace components { namespace parser { namespace json {

    class qor_pp_module_interface(QOR_JSON) escapeSequence : public Sequence
    {
    public: 
        escapeSequence(Parser* parser);
        virtual ~escapeSequence();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_ESCAPESEQUENCE
