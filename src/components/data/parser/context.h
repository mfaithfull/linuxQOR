// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_CONTEXT
#define QOR_PP_H_DATA_PARSER_CONTEXT

#include "src/platform/compiler/compiler.h"
#include "src/framework/data/pipeline/buffercontext.h"

namespace qor { namespace data { namespace parser {

    using CodePointContext = pipeline::BufferContext<uint32_t>;
    using ByteContext = pipeline::BufferContext<byte>;
}}}//qor::data::parser

#endif//QOR_PP_H_DATA_PARSER_CONTEXT
