// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_UNICODE_FUNCS
#define QOR_PP_H_TEXT_UNICODE_FUNCS

#include "codepoint.h"
#include "../shaping/common.h"
#include "../shaping/algs.h"
#include "generalcategory.h"
#include "combiningclass.h"
#include "scripts.h"

namespace qor{ namespace unicode{

    class IUnicodeFunctions
    {
        virtual combining_class_t combining_class(codepoint_t _unicode) = 0;
        virtual unsigned int eastasian_width(codepoint_t _unicode) = 0;
        virtual general_category_t general_category(codepoint_t _unicode) = 0;
        virtual codepoint_t mirroring(codepoint_t _unicode) = 0;
        virtual script script(codepoint_t _unicode) = 0;
        virtual bool compose(codepoint_t a, codepoint_t b, codepoint_t* ab) = 0;
        virtual bool decompose(codepoint_t ab, codepoint_t* a, codepoint_t* b) = 0;
        virtual unsigned int decompose_compatability(codepoint_t u, codepoint_t* decomposed) = 0;
    };

struct unicode_funcs_t;

typedef combining_class_t(*unicode_combining_class_func_t)(unicode_funcs_t* ufuncs, codepoint_t _unicode, void * user_data);
typedef unsigned int(*unicode_eastasian_width_func_t)(unicode_funcs_t* ufuncs, codepoint_t _unicode, void* user_data);
typedef general_category_t(*unicode_general_category_func_t)(unicode_funcs_t* ufuncs, codepoint_t _unicode, void* user_data);
typedef codepoint_t(*unicode_mirroring_func_t)(unicode_funcs_t* ufuncs, codepoint_t _unicode, void* user_data);
typedef script(*unicode_script_func_t)(unicode_funcs_t* ufuncs, codepoint_t _unicode, void* user_data);
typedef qb::bool_t(*unicode_compose_func_t)(unicode_funcs_t* ufuncs, codepoint_t a, codepoint_t b, codepoint_t* ab, void* user_data);
typedef qb::bool_t(*unicode_decompose_func_t)(unicode_funcs_t* ufuncs, codepoint_t ab, codepoint_t* a, codepoint_t* b, void* user_data);
typedef unsigned int(*unicode_decompose_compatibility_func_t)(unicode_funcs_t* ufuncs, codepoint_t u, codepoint_t* decomposed, void* user_data);

struct unicode_funcs_t
{
    //qb::object_header_t header;
    unicode_funcs_t* parent;

    combining_class_t combining_class (codepoint_t _unicode){ return func.combining_class (this, _unicode, user_data.combining_class); }

    unsigned int eastasian_width (codepoint_t _unicode){ return func.eastasian_width (this, _unicode, user_data.eastasian_width); }

    general_category_t general_category (codepoint_t _unicode){ return func.general_category (this, _unicode, user_data.general_category); }

    codepoint_t mirroring (codepoint_t _unicode){ return func.mirroring (this, _unicode, user_data.mirroring); }

    script script(codepoint_t _unicode){ return func.script (this, _unicode, user_data.script); }

    qb::bool_t compose(codepoint_t a, codepoint_t b, codepoint_t* ab)
    {
        *ab = 0;
        if (!a || !b) return false;
        return func.compose(this, a, b, ab, user_data.compose);
    }

    qb::bool_t decompose(codepoint_t ab, codepoint_t* a, codepoint_t* b)
    {
        *a = ab; *b = 0;
        return func.decompose(this, ab, a, b, user_data.decompose);
    }

    unsigned int decompose_compatibility(codepoint_t u, codepoint_t* decomposed)
    {
        unsigned int ret = func.decompose_compatibility(this, u, decomposed, user_data.decompose_compatibility);
        if(ret == 1 && u == decomposed[0]) 
        {
            decomposed[0] = 0;
            return 0;
        }
        decomposed[ret] = 0;
        return ret;
    }

    unsigned int modified_combining_class(codepoint_t u)
    {
        /* Reorder SAKOT to ensure it comes after any tone marks. */
        if (u == 0x1A60u) return 254;
        /* Reorder PADMA to ensure it comes after any vowel marks. */
        if (u == 0x0FC6u) return 254;
        /* Reorder TSA -PHRU to reorder before U+0F74 */
        if (u == 0x0F39u) return 127;

        return _hb_modified_combining_class[static_cast<int>(combining_class(u))];
    }

    static qb::bool_t is_variation_selector(codepoint_t unicode)
    {
        /* U+180B..180D, U+180F MONGOLIAN FREE VARIATION SELECTORs are handled in the
            * Arabic shaper.  No need to match them here. */
        return qb::hb_in_ranges<codepoint_t> (unicode, 0xFE00u, 0xFE0Fu, /* VARIATION SELECTOR-1..16 */
                            0xE0100u, 0xE01EFu);  /* VARIATION SELECTOR-17..256 */
    }

    /* Default_Ignorable codepoints:
    *
    * Note: While U+115F, U+1160, U+3164 and U+FFA0 are Default_Ignorable,
    * we do NOT want to hide them, as the way Uniscribe has implemented them
    * is with regular spacing glyphs, and that's the way fonts are made to work.
    * As such, we make exceptions for those four.
    * Also ignoring U+1BCA0..1BCA3. https://github.com/harfbuzz/harfbuzz/issues/503
    *
    * Unicode 14.0:
    * $ grep '; Default_Ignorable_Code_Point ' DerivedCoreProperties.txt | sed 's/;.*#/#/'
    * 00AD          # Cf       SOFT HYPHEN
    * 034F          # Mn       COMBINING GRAPHEME JOINER
    * 061C          # Cf       ARABIC LETTER MARK
    * 115F..1160    # Lo   [2] HANGUL CHOSEONG FILLER..HANGUL JUNGSEONG FILLER
    * 17B4..17B5    # Mn   [2] KHMER VOWEL INHERENT AQ..KHMER VOWEL INHERENT AA
    * 180B..180D    # Mn   [3] MONGOLIAN FREE VARIATION SELECTOR ONE..MONGOLIAN FREE VARIATION SELECTOR THREE
    * 180E          # Cf       MONGOLIAN VOWEL SEPARATOR
    * 180F          # Mn       MONGOLIAN FREE VARIATION SELECTOR FOUR
    * 200B..200F    # Cf   [5] ZERO WIDTH SPACE..RIGHT-TO-LEFT MARK
    * 202A..202E    # Cf   [5] LEFT-TO-RIGHT EMBEDDING..RIGHT-TO-LEFT OVERRIDE
    * 2060..2064    # Cf   [5] WORD JOINER..INVISIBLE PLUS
    * 2065          # Cn       <reserved-2065>
    * 2066..206F    # Cf  [10] LEFT-TO-RIGHT ISOLATE..NOMINAL DIGIT SHAPES
    * 3164          # Lo       HANGUL FILLER
    * FE00..FE0F    # Mn  [16] VARIATION SELECTOR-1..VARIATION SELECTOR-16
    * FEFF          # Cf       ZERO WIDTH NO-BREAK SPACE
    * FFA0          # Lo       HALFWIDTH HANGUL FILLER
    * FFF0..FFF8    # Cn   [9] <reserved-FFF0>..<reserved-FFF8>
    * 1BCA0..1BCA3  # Cf   [4] SHORTHAND FORMAT LETTER OVERLAP..SHORTHAND FORMAT UP STEP
    * 1D173..1D17A  # Cf   [8] MUSICAL SYMBOL BEGIN BEAM..MUSICAL SYMBOL END PHRASE
    * E0000         # Cn       <reserved-E0000>
    * E0001         # Cf       LANGUAGE TAG
    * E0002..E001F  # Cn  [30] <reserved-E0002>..<reserved-E001F>
    * E0020..E007F  # Cf  [96] TAG SPACE..CANCEL TAG
    * E0080..E00FF  # Cn [128] <reserved-E0080>..<reserved-E00FF>
    * E0100..E01EF  # Mn [240] VARIATION SELECTOR-17..VARIATION SELECTOR-256
    * E01F0..E0FFF  # Cn [3600] <reserved-E01F0>..<reserved-E0FFF>
    */

    static qb::bool_t is_default_ignorable(codepoint_t ch)
    {
        codepoint_t plane = ch >> 16;
        if (plane == 0)
        {
            // BMP
            codepoint_t page = ch >> 8;
            switch (page) 
            {
            case 0x00: return ch == 0x00ADu;
            case 0x03: return ch == 0x034Fu;
            case 0x06: return ch == 0x061Cu;
            case 0x17: return qb::hb_in_range<codepoint_t> (ch, 0x17B4u, 0x17B5u);
            case 0x18: return qb::hb_in_range<codepoint_t> (ch, 0x180Bu, 0x180Eu);
            case 0x20: return qb::hb_in_ranges<codepoint_t> (ch, 0x200Bu, 0x200Fu, 0x202Au, 0x202Eu, 0x2060u, 0x206Fu);
            case 0xFE: return qb::hb_in_range<codepoint_t> (ch, 0xFE00u, 0xFE0Fu) || ch == 0xFEFFu;
            case 0xFF: return qb::hb_in_range<codepoint_t> (ch, 0xFFF0u, 0xFFF8u);
            default: return false;
            }
        }
        else
        {
            // Other planes
            switch (plane) 
            {
            case 0x01: return qb::hb_in_range<codepoint_t> (ch, 0x1D173u, 0x1D17Au);
            case 0x0E: return qb::hb_in_range<codepoint_t> (ch, 0xE0000u, 0xE0FFFu);
            default: return false;
            }
        }
    }

    /* Space estimates based on:
    * https://unicode.org/charts/PDF/U2000.pdf
    * https://docs.microsoft.com/en-us/typography/develop/character-design-standards/whitespace
    */
    enum space_t 
    {
        NOT_SPACE = 0,
        SPACE_EM   = 1,
        SPACE_EM_2 = 2,
        SPACE_EM_3 = 3,
        SPACE_EM_4 = 4,
        SPACE_EM_5 = 5,
        SPACE_EM_6 = 6,
        SPACE_EM_16 = 16,
        SPACE_4_EM_18,	/* 4/18th of an EM! */
        SPACE,
        SPACE_FIGURE,
        SPACE_PUNCTUATION,
        SPACE_NARROW,
    };

    static space_t space_fallback_type(codepoint_t u)
    {
        switch (u)
        {
            /* All GC=Zs chars that can use a fallback. */
            default:	    return NOT_SPACE;	/* U+1680 OGHAM SPACE MARK */
            case 0x0020u: return SPACE;	/* U+0020 SPACE */
            case 0x00A0u: return SPACE;	/* U+00A0 NO-BREAK SPACE */
            case 0x2000u: return SPACE_EM_2;	/* U+2000 EN QUAD */
            case 0x2001u: return SPACE_EM;	/* U+2001 EM QUAD */
            case 0x2002u: return SPACE_EM_2;	/* U+2002 EN SPACE */
            case 0x2003u: return SPACE_EM;	/* U+2003 EM SPACE */
            case 0x2004u: return SPACE_EM_3;	/* U+2004 THREE-PER-EM SPACE */
            case 0x2005u: return SPACE_EM_4;	/* U+2005 FOUR-PER-EM SPACE */
            case 0x2006u: return SPACE_EM_6;	/* U+2006 SIX-PER-EM SPACE */
            case 0x2007u: return SPACE_FIGURE;	/* U+2007 FIGURE SPACE */
            case 0x2008u: return SPACE_PUNCTUATION;	/* U+2008 PUNCTUATION SPACE */
            case 0x2009u: return SPACE_EM_5;		/* U+2009 THIN SPACE */
            case 0x200Au: return SPACE_EM_16;		/* U+200A HAIR SPACE */
            case 0x202Fu: return SPACE_NARROW;	/* U+202F NARROW NO-BREAK SPACE */
            case 0x205Fu: return SPACE_4_EM_18;	/* U+205F MEDIUM MATHEMATICAL SPACE */
            case 0x3000u: return SPACE_EM;		/* U+3000 IDEOGRAPHIC SPACE */
        }
    }

    static codepoint_t vertical_char_for(codepoint_t u)
    {
        switch (u >> 8)
        {
            case 0x20: switch (u) {
        case 0x2013u: return 0xfe32u; // EN DASH
        case 0x2014u: return 0xfe31u; // EM DASH
        case 0x2025u: return 0xfe30u; // TWO DOT LEADER
        case 0x2026u: return 0xfe19u; // HORIZONTAL ELLIPSIS
            } break;
            case 0x30: switch (u) {
        case 0x3001u: return 0xfe11u; // IDEOGRAPHIC COMMA
        case 0x3002u: return 0xfe12u; // IDEOGRAPHIC FULL STOP
        case 0x3008u: return 0xfe3fu; // LEFT ANGLE BRACKET
        case 0x3009u: return 0xfe40u; // RIGHT ANGLE BRACKET
        case 0x300au: return 0xfe3du; // LEFT DOUBLE ANGLE BRACKET
        case 0x300bu: return 0xfe3eu; // RIGHT DOUBLE ANGLE BRACKET
        case 0x300cu: return 0xfe41u; // LEFT CORNER BRACKET
        case 0x300du: return 0xfe42u; // RIGHT CORNER BRACKET
        case 0x300eu: return 0xfe43u; // LEFT WHITE CORNER BRACKET
        case 0x300fu: return 0xfe44u; // RIGHT WHITE CORNER BRACKET
        case 0x3010u: return 0xfe3bu; // LEFT BLACK LENTICULAR BRACKET
        case 0x3011u: return 0xfe3cu; // RIGHT BLACK LENTICULAR BRACKET
        case 0x3014u: return 0xfe39u; // LEFT TORTOISE SHELL BRACKET
        case 0x3015u: return 0xfe3au; // RIGHT TORTOISE SHELL BRACKET
        case 0x3016u: return 0xfe17u; // LEFT WHITE LENTICULAR BRACKET
        case 0x3017u: return 0xfe18u; // RIGHT WHITE LENTICULAR BRACKET
            } break;
            case 0xfe: switch (u) {
        case 0xfe4fu: return 0xfe34u; // WAVY LOW LINE
            } break;
            case 0xff: switch (u) {
                case 0xff01u: return 0xfe15u; // FULLWIDTH EXCLAMATION MARK
                case 0xff08u: return 0xfe35u; // FULLWIDTH LEFT PARENTHESIS
                case 0xff09u: return 0xfe36u; // FULLWIDTH RIGHT PARENTHESIS
                case 0xff0cu: return 0xfe10u; // FULLWIDTH COMMA
                case 0xff1au: return 0xfe13u; // FULLWIDTH COLON
                case 0xff1bu: return 0xfe14u; // FULLWIDTH SEMICOLON
                case 0xff1fu: return 0xfe16u; // FULLWIDTH QUESTION MARK
                case 0xff3bu: return 0xfe47u; // FULLWIDTH LEFT SQUARE BRACKET
                case 0xff3du: return 0xfe48u; // FULLWIDTH RIGHT SQUARE BRACKET
                case 0xff3fu: return 0xfe33u; // FULLWIDTH LOW LINE
                case 0xff5bu: return 0xfe37u; // FULLWIDTH LEFT CURLY BRACKET
                case 0xff5du: return 0xfe38u; // FULLWIDTH RIGHT CURLY BRACKET
            } break;
        }

        return u;
    }

    struct 
    {
        unicode_combining_class_func_t combining_class;
        unicode_eastasian_width_func_t eastasian_width;
        unicode_general_category_func_t general_category;
        unicode_mirroring_func_t mirroring;
        unicode_script_func_t script;
        unicode_compose_func_t compose;
        unicode_decompose_func_t decompose;
        unicode_decompose_compatibility_func_t decompose_compatibility;
    } func;

    struct 
    {
        void* combining_class;
        void* eastasian_width;
        void* general_category;
        void* mirroring;
        void* script;
        void* compose;
        void* decompose;
        void* decompose_compatibility;
    } user_data;

    struct 
    {
        qb::destroy_func_t combining_class;        
        qb::destroy_func_t eastasian_width;
        qb::destroy_func_t general_category;
        qb::destroy_func_t mirroring;
        qb::destroy_func_t script;
        qb::destroy_func_t compose;
        qb::destroy_func_t decompose;
        qb::destroy_func_t decompose_compatibility;
    } destroy;
};

}}//qor::unicode

#endif//QOR_PP_H_TEXT_UNICODE_FUNCS