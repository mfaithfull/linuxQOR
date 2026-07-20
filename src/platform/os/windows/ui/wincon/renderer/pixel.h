// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_GRAPHCON_RENDERER_PIXEL
#define QOR_PP_H_OS_WINDOWS_UI_GRAPHCON_RENDERER_PIXEL

namespace qor { namespace ui { namespace win {

    struct Pixel 
    {
        union {
            wchar_t UnicodeChar;
            char   AsciiChar;
        } Char{0};
        unsigned short attributes{0};
    };

}}}

#endif//QOR_PP_H_OS_WINDOWS_UI_GRAPHCON_RENDERER_PIXEL