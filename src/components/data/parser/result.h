// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_RESULT
#define QOR_PP_H_DATA_PARSER_RESULT

#include <cstdint>
#include "src/platform/compiler/compiler.h"
#include "tokens.h"

namespace qor { namespace data { namespace parser {

    class Result
    {
    public:

        enum eResultCode
        {
            UNINITIALIZED,
            MORE_DATA,
            FAILURE,
            SUCCESS,
        };

        eResultCode code{UNINITIALIZED};
        byte first{0};
        size_t length{0};
        size_t m_position{0};
        uint64_t token{0};
    };
    
}}}//qor::data::parser

#endif//QOR_PP_H_DATA_PARSER_RESULT