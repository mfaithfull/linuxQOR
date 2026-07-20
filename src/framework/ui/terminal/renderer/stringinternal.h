// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_STRING_INTERNAL
#define QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_STRING_INTERNAL

#include <cstdint>
#include <string>
#include <vector>

namespace qor{ namespace ui{ namespace tty {

    bool EatCodePoint(std::string_view input, size_t start, size_t* end, uint32_t* ucs);
    bool EatCodePoint(std::wstring_view input, size_t start, size_t* end, uint32_t* ucs);

    bool IsCombining(uint32_t ucs);
    bool IsFullWidth(uint32_t ucs);
    bool IsControl(uint32_t ucs);

    size_t GlyphPrevious(std::string_view input, size_t start);
    size_t GlyphNext(std::string_view input, size_t start);

    // Return the index in the |input| string of the glyph at |glyph_offset|, starting at |start|
    size_t GlyphIterate(std::string_view input, int glyph_offset, size_t start = 0);

    // Returns the number of glyphs in |input|.
    int GlyphCount(std::string_view input);

    // Properties from: https://www.unicode.org/Public/UCD/latest/ucd/auxiliary/WordBreakProperty.txt
    enum class WordBreakProperty : int8_t 
    {
        ALetter,
        CR,
        Double_Quote,
        Extend,
        ExtendNumLet,
        Format,
        Hebrew_Letter,
        Katakana,
        LF,
        MidLetter,
        MidNum,
        MidNumLet,
        Newline,
        Numeric,
        Regional_Indicator,
        Single_Quote,
        WSegSpace,
        ZWJ,
    };

    WordBreakProperty CodepointToWordBreakProperty(uint32_t codepoint);
    std::vector<WordBreakProperty> Utf8ToWordBreakProperty(std::string_view input);
    bool IsWordBreakingCharacter(std::string_view input, size_t glyph_index);

}}}//qor::ui::tty

#endif//QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_STRING_INTERNAL
