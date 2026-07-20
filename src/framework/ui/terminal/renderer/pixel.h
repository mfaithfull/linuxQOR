// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_PIXEL
#define QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_PIXEL

#include <cstdint>
#include <string>
#include "colour.h"

namespace qor{ namespace ui{ namespace tty {

    struct Pixel 
    {
        Pixel()
        : blink(false),
        bold(false),
        dim(false),
        italic(false),
        inverted(false),
        underlined(false),
        underlined_double(false),
        strikethrough(false),
        automerge(false) {}

        // A bit field representing the style:
        bool blink : 1;
        bool bold : 1;
        bool dim : 1;
        bool italic : 1;
        bool inverted : 1;
        bool underlined : 1;
        bool underlined_double : 1;
        bool strikethrough : 1;
        bool automerge : 1;

        // The hyperlink associated with the pixel.
        // 0 is the default value, meaning no hyperlink.
        // It's an index for accessing Screen meta data
        uint8_t hyperlink = 0;

        // The graphemes stored into the pixel. To support combining characters,
        // like: a?, this can potentially contain multiple codepoints.
        std::string character = "";

        // Colours:
        Colour background_color = Colour::Default;
        Colour foreground_color = Colour::Default;
    };

}}}//qor::ui::tty

#endif//QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_PIXEL