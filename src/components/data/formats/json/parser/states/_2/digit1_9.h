// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_DIGIT1_9
#define QOR_PP_H_DATA_JSON_PARSER_DIGIT1_9

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    class qor_pp_module_interface(QOR_JSON) digit1_9 : public OneOfARange_t<uint32_t>
    {
        public: digit1_9(Parser* parser);
        virtual ~digit1_9();
        virtual void Emit();
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_DIGIT1_9
