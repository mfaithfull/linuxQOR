// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_UNICODE_RANGE
#define QOR_PP_H_TEXT_UNICODE_RANGE

#include "codepoint.h"

namespace qor{ namespace unicode{

    struct range_t
    {
        static int cmp(const void* _key, const void* _item)
        {
            codepoint_t cp = *((codepoint_t *) _key);
            const range_t* range = (range_t*) _item;

            if (cp < range->start)
                return -1;
            else if (cp <= range->end)
                return 0;
            else
                return +1;
        }

        codepoint_t start;
        codepoint_t end;
    };
    
}}//qor::unicode

#endif//QOR_PP_H_TEXT_UNICODE_RANGE