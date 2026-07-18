// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WIN_UI_TERMINAL
#define QOR_PP_H_WIN_UI_TERMINAL

#include <cstdint>
#include <string>
#include <vector>

namespace qor{ namespace ui{ namespace tui { namespace win{

    struct Dimensions
    {
        int dimx;
        int dimy;
    };

    class Terminal 
    {
    public:
    
        static Dimensions Size();
        static void SetFallbackSize(const Dimensions& fallbackSize);

        enum Palette 
        {
            Palette1,
            Palette16,
            Palette256,
            TrueColour,
        };

        static Palette ColourSupport();
        static void SetColourSupport(Palette palette);
        uint8_t RegisterHyperlink(std::string_view link);
        const std::string& Hyperlink(uint8_t id) const;

    protected:

        std::vector<std::string> hyperlinks_ = {""};    
    };

}}}}//qor::ui::tui::win

#endif//QOR_PP_H_WIN_UI_TERMINAL
