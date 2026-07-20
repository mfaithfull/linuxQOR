// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_STRING
#define QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_STRING

#include <string>
#include <string_view>
#include <vector>

namespace qor{ namespace ui{ namespace tty {

    std::string to_string(std::wstring_view s);
    std::wstring to_wstring(std::string_view s);

    template <typename T>
    std::wstring to_wstring(T s) 
    {
        return to_wstring(std::string_view(std::to_string(s)));
    }

    template <>
    inline std::wstring to_wstring(const char* s) 
    {
        return to_wstring(std::string_view(s));
    }

    int string_width(std::string_view);

    // Split the string into a its glyphs. An empty one is inserted ater fullwidth ones.
    std::vector<std::string> Utf8ToGlyphs(std::string_view input);

    // Map every cells drawn by |input| to their corresponding Glyphs. Half-size
    // Glyphs takes one cell, full-size Glyphs take two cells.
    std::vector<int> CellToGlyphIndex(std::string_view input);

}}}//qor::ui::tty

#endif//QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_STRING

