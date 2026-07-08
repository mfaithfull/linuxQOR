// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_PARSER_WS
#define QOR_PP_H_COMPONENTS_JSON_PARSER_WS

#include "../tokens.h"

namespace qor { namespace components { namespace parser { namespace json {

    /*ws = *(
              %x20 /              ; Space
              %x09 /              ; Horizontal tab
              %x0A /              ; Line feed or New line
              %x0D )              ; Carriage return*/
    class ws : public ZeroOrMore
    {
    public: 
        ws(Parser* parser) :
            ZeroOrMore(parser,
                new_ref<AnyOneOf>(parser,
                    new_ref<WSP>(parser),
                    new_ref<AnyOneOf>(parser,
                        new_ref<CR>(parser),
                        new_ref<LF>(parser)
                    )
                ),
            static_cast<uint64_t>(jsonToken::ws)){ }
        virtual ~ws() = default;
        virtual void Emit(){ };//Emit nothing
    };

}}}}//qor::components::parser::json

#endif//QOR_PP_H_COMPONENTS_JSON_PARSER_WS
