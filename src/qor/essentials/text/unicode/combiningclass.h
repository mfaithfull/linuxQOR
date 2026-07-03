// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_UNICODE_COMBININGCLASS
#define QOR_PP_H_TEXT_UNICODE_COMBININGCLASS

#include <stdint.h>

namespace qor{ namespace unicode{

    enum class combining_class_t
    {
        NOT_REORDERED	= 0,
        OVERLAY		= 1,
        NUKTA		= 7,
        KANA_VOICING	= 8,
        VIRAMA		= 9,

        /* Hebrew */
        CCC10	=  10,
        CCC11	=  11,
        CCC12	=  12,
        CCC13	=  13,
        CCC14	=  14,
        CCC15	=  15,
        CCC16	=  16,
        CCC17	=  17,
        CCC18	=  18,
        CCC19	=  19,
        CCC20	=  20,
        CCC21	=  21,
        CCC22	=  22,
        CCC23	=  23,
        CCC24	=  24,
        CCC25	=  25,
        CCC26	=  26,

        /* Arabic */
        CCC27	=  27,
        CCC28	=  28,
        CCC29	=  29,
        CCC30	=  30,
        CCC31	=  31,
        CCC32	=  32,
        CCC33	=  33,
        CCC34	=  34,
        CCC35	=  35,

        /* Syriac */
        CCC36	=  36,

        /* Telugu */
        CCC84	=  84,
        CCC91	=  91,

        /* Thai */
        CCC103	= 103,
        CCC107	= 107,

        /* Lao */
        CCC118	= 118,
        CCC122	= 122,

        /* Tibetan */
        CCC129	= 129,
        CCC130	= 130,
        CCC132	= 132,

        ATTACHED_BELOW_LEFT	= 200,
        ATTACHED_BELOW		= 202,
        ATTACHED_ABOVE		= 214,
        ATTACHED_ABOVE_RIGHT	= 216,
        BELOW_LEFT			= 218,
        BELOW			= 220,
        BELOW_RIGHT		= 222,
        LEFT			= 224,
        RIGHT			= 226,
        ABOVE_LEFT			= 228,
        ABOVE			= 230,
        ABOVE_RIGHT		= 232,
        DOUBLE_BELOW		= 233,
        DOUBLE_ABOVE		= 234,

        IOTA_SUBSCRIPT		= 240,

        INVALID	= 255
    };

/* Hebrew
 *
 * We permute the "fixed-position" classes 10-26 into the order
 * described in the SBL Hebrew manual:
 *
 * https://www.sbl-site.org/Fonts/SBLHebrewUserManual1.5x.pdf
 *
 * (as recommended by:
 *  https://forum.fontlab.com/archive-old-microsoft-volt-group/vista-and-diacritic-ordering/msg22823/)
 *
 * More details here:
 * https://bugzilla.mozilla.org/show_bug.cgi?id=662055
 */
#define HB_MODIFIED_COMBINING_CLASS_CCC10 22 /* sheva */
#define HB_MODIFIED_COMBINING_CLASS_CCC11 15 /* hataf segol */
#define HB_MODIFIED_COMBINING_CLASS_CCC12 16 /* hataf patah */
#define HB_MODIFIED_COMBINING_CLASS_CCC13 17 /* hataf qamats */
#define HB_MODIFIED_COMBINING_CLASS_CCC14 23 /* hiriq */
#define HB_MODIFIED_COMBINING_CLASS_CCC15 18 /* tsere */
#define HB_MODIFIED_COMBINING_CLASS_CCC16 19 /* segol */
#define HB_MODIFIED_COMBINING_CLASS_CCC17 20 /* patah */
#define HB_MODIFIED_COMBINING_CLASS_CCC18 21 /* qamats & qamats qatan */
#define HB_MODIFIED_COMBINING_CLASS_CCC19 14 /* holam & holam haser for vav*/
#define HB_MODIFIED_COMBINING_CLASS_CCC20 24 /* qubuts */
#define HB_MODIFIED_COMBINING_CLASS_CCC21 12 /* dagesh */
#define HB_MODIFIED_COMBINING_CLASS_CCC22 25 /* meteg */
#define HB_MODIFIED_COMBINING_CLASS_CCC23 13 /* rafe */
#define HB_MODIFIED_COMBINING_CLASS_CCC24 10 /* shin dot */
#define HB_MODIFIED_COMBINING_CLASS_CCC25 11 /* sin dot */
#define HB_MODIFIED_COMBINING_CLASS_CCC26 26 /* point varika */

/*
 * Arabic
 *
 * Modify to move Shadda (ccc=33) before other marks.  See:
 * https://unicode.org/faq/normalization.html#8
 * https://unicode.org/faq/normalization.html#9
 */
#define HB_MODIFIED_COMBINING_CLASS_CCC27 28 /* fathatan */
#define HB_MODIFIED_COMBINING_CLASS_CCC28 29 /* dammatan */
#define HB_MODIFIED_COMBINING_CLASS_CCC29 30 /* kasratan */
#define HB_MODIFIED_COMBINING_CLASS_CCC30 31 /* fatha */
#define HB_MODIFIED_COMBINING_CLASS_CCC31 32 /* damma */
#define HB_MODIFIED_COMBINING_CLASS_CCC32 33 /* kasra */
#define HB_MODIFIED_COMBINING_CLASS_CCC33 27 /* shadda */
#define HB_MODIFIED_COMBINING_CLASS_CCC34 34 /* sukun */
#define HB_MODIFIED_COMBINING_CLASS_CCC35 35 /* superscript alef */

/* Syriac */
#define HB_MODIFIED_COMBINING_CLASS_CCC36 36 /* superscript alaph */

/* Telugu
 *
 * Modify Telugu length marks (ccc=84, ccc=91).
 * These are the only matras in the main Indic scripts range that have
 * a non-zero ccc.  That makes them reorder with the Halant (ccc=9).
 * Assign 4 and 5, which are otherwise unassigned.
 */
#define HB_MODIFIED_COMBINING_CLASS_CCC84 4 /* length mark */
#define HB_MODIFIED_COMBINING_CLASS_CCC91 5 /* ai length mark */

/* Thai
 *
 * Modify U+0E38 and U+0E39 (ccc=103) to be reordered before U+0E3A (ccc=9).
 * Assign 3, which is unassigned otherwise.
 * Uniscribe does this reordering too.
 */
#define HB_MODIFIED_COMBINING_CLASS_CCC103 3 /* sara u / sara uu */
#define HB_MODIFIED_COMBINING_CLASS_CCC107 107 /* mai * */

/* Lao */
#define HB_MODIFIED_COMBINING_CLASS_CCC118 118 /* sign u / sign uu */
#define HB_MODIFIED_COMBINING_CLASS_CCC122 122 /* mai * */

/* Tibetan
 *
 * In case of multiple vowel-signs, use u first (but after achung)
 * this allows Dzongkha multi-vowel shortcuts to render correctly
 */
#define HB_MODIFIED_COMBINING_CLASS_CCC129 129 /* sign aa */
#define HB_MODIFIED_COMBINING_CLASS_CCC130 132 /* sign i */
#define HB_MODIFIED_COMBINING_CLASS_CCC132 131 /* sign u */


    const uint8_t _hb_modified_combining_class[256] =
    {
        0, /* HB_UNICODE_COMBINING_CLASS_NOT_REORDERED */
        1, /* HB_UNICODE_COMBINING_CLASS_OVERLAY */
        2, 3, 4, 5, 6,
        7, /* HB_UNICODE_COMBINING_CLASS_NUKTA */
        8, /* HB_UNICODE_COMBINING_CLASS_KANA_VOICING */
        9, /* HB_UNICODE_COMBINING_CLASS_VIRAMA */

        /* Hebrew */
        HB_MODIFIED_COMBINING_CLASS_CCC10,
        HB_MODIFIED_COMBINING_CLASS_CCC11,
        HB_MODIFIED_COMBINING_CLASS_CCC12,
        HB_MODIFIED_COMBINING_CLASS_CCC13,
        HB_MODIFIED_COMBINING_CLASS_CCC14,
        HB_MODIFIED_COMBINING_CLASS_CCC15,
        HB_MODIFIED_COMBINING_CLASS_CCC16,
        HB_MODIFIED_COMBINING_CLASS_CCC17,
        HB_MODIFIED_COMBINING_CLASS_CCC18,
        HB_MODIFIED_COMBINING_CLASS_CCC19,
        HB_MODIFIED_COMBINING_CLASS_CCC20,
        HB_MODIFIED_COMBINING_CLASS_CCC21,
        HB_MODIFIED_COMBINING_CLASS_CCC22,
        HB_MODIFIED_COMBINING_CLASS_CCC23,
        HB_MODIFIED_COMBINING_CLASS_CCC24,
        HB_MODIFIED_COMBINING_CLASS_CCC25,
        HB_MODIFIED_COMBINING_CLASS_CCC26,

        /* Arabic */
        HB_MODIFIED_COMBINING_CLASS_CCC27,
        HB_MODIFIED_COMBINING_CLASS_CCC28,
        HB_MODIFIED_COMBINING_CLASS_CCC29,
        HB_MODIFIED_COMBINING_CLASS_CCC30,
        HB_MODIFIED_COMBINING_CLASS_CCC31,
        HB_MODIFIED_COMBINING_CLASS_CCC32,
        HB_MODIFIED_COMBINING_CLASS_CCC33,
        HB_MODIFIED_COMBINING_CLASS_CCC34,
        HB_MODIFIED_COMBINING_CLASS_CCC35,

        /* Syriac */
        HB_MODIFIED_COMBINING_CLASS_CCC36,

        37, 38, 39,
        40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
        60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
        80, 81, 82, 83,

        /* Telugu */
        HB_MODIFIED_COMBINING_CLASS_CCC84,
        85, 86, 87, 88, 89, 90,
        HB_MODIFIED_COMBINING_CLASS_CCC91,
        92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,

        /* Thai */
        HB_MODIFIED_COMBINING_CLASS_CCC103,
        104, 105, 106,
        HB_MODIFIED_COMBINING_CLASS_CCC107,
        108, 109, 110, 111, 112, 113, 114, 115, 116, 117,

        /* Lao */
        HB_MODIFIED_COMBINING_CLASS_CCC118,
        119, 120, 121,
        HB_MODIFIED_COMBINING_CLASS_CCC122,
        123, 124, 125, 126, 127, 128,

        /* Tibetan */
        HB_MODIFIED_COMBINING_CLASS_CCC129,
        HB_MODIFIED_COMBINING_CLASS_CCC130,
        131,
        HB_MODIFIED_COMBINING_CLASS_CCC132,
        133, 134, 135, 136, 137, 138, 139,


        140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
        150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
        160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
        170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
        180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
        190, 191, 192, 193, 194, 195, 196, 197, 198, 199,

        200, /* HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_LEFT */
        201,
        202, /* HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW */
        203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
        214, /* HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE */
        215,
        216, /* HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_RIGHT */
        217,
        218, /* HB_UNICODE_COMBINING_CLASS_BELOW_LEFT */
        219,
        220, /* HB_UNICODE_COMBINING_CLASS_BELOW */
        221,
        222, /* HB_UNICODE_COMBINING_CLASS_BELOW_RIGHT */
        223,
        224, /* HB_UNICODE_COMBINING_CLASS_LEFT */
        225,
        226, /* HB_UNICODE_COMBINING_CLASS_RIGHT */
        227,
        228, /* HB_UNICODE_COMBINING_CLASS_ABOVE_LEFT */
        229,
        230, /* HB_UNICODE_COMBINING_CLASS_ABOVE */
        231,
        232, /* HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT */
        233, /* HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW */
        234, /* HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE */
        235, 236, 237, 238, 239,
        240, /* HB_UNICODE_COMBINING_CLASS_IOTA_SUBSCRIPT */
        241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
        255, /* HB_UNICODE_COMBINING_CLASS_INVALID */
    };

}}//qor::unicode

#endif//QOR_PP_H_TEXT_UNICODE_COMBININGCLASS