// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_COLOURINFO
#define QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_COLOURINFO

#include "colour.h"

namespace qor{ namespace ui{ namespace tty {

    struct ColourInfo 
    {
        const char* name;
        uint8_t index_256;
        uint8_t index_16;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t hue;
        uint8_t saturation;
        uint8_t value;
    };

    qor_pp_module_interface(QOR_TERMINAL) ColourInfo GetColourInfo(Colour::Palette256 index);
    qor_pp_module_interface(QOR_TERMINAL) ColourInfo GetColourInfo(Colour::Palette16 index);

}}}//qor::ui::tty

#endif//QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_COLOURINFO
