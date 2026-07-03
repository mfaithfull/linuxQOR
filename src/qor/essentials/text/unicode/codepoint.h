// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_UNICODE_CODEPOINT
#define QOR_PP_H_TEXT_UNICODE_CODEPOINT

#include <stdint.h>

namespace qor{ namespace unicode{

    typedef uint32_t codepoint_t;

    constexpr unsigned int UNICODE_MAX = 0x10FFFFu;

}}//qor::unicode

#endif//QOR_PP_H_TEXT_UNICODE_CODEPOINT