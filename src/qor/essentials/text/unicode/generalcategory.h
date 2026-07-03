// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_UNICODE_GENERALCATEGORY
#define QOR_PP_H_TEXT_UNICODE_GENERALCATEGORY

namespace qor{ namespace unicode{

    // Unicode Character Database property: General_Category (gc)
    enum class general_category_t
    {
        CONTROL,			    /* Cc */
        FORMAT,			        /* Cf */
        UNASSIGNED,		        /* Cn */
        PRIVATE_USE,		    /* Co */
        SURROGATE,		        /* Cs */
        LOWERCASE_LETTER,		/* Ll */
        MODIFIER_LETTER,		/* Lm */
        OTHER_LETTER,		    /* Lo */
        TITLECASE_LETTER,		/* Lt */
        UPPERCASE_LETTER,		/* Lu */
        SPACING_MARK,		    /* Mc */
        ENCLOSING_MARK,		    /* Me */
        NON_SPACING_MARK,		/* Mn */
        DECIMAL_NUMBER,		    /* Nd */
        LETTER_NUMBER,		    /* Nl */
        OTHER_NUMBER,		    /* No */
        CONNECT_PUNCTUATION,	/* Pc */
        DASH_PUNCTUATION,		/* Pd */
        CLOSE_PUNCTUATION,	    /* Pe */
        FINAL_PUNCTUATION,	    /* Pf */
        INITIAL_PUNCTUATION,	/* Pi */
        OTHER_PUNCTUATION,	    /* Po */
        OPEN_PUNCTUATION,		/* Ps */
        CURRENCY_SYMBOL,		/* Sc */
        MODIFIER_SYMBOL,		/* Sk */
        MATH_SYMBOL,		    /* Sm */
        OTHER_SYMBOL,		    /* So */
        LINE_SEPARATOR,		    /* Zl */
        PARAGRAPH_SEPARATOR,	/* Zp */
        SPACE_SEPARATOR		    /* Zs */
    };

}}//qor::unicode

#endif//QOR_PP_H_TEXT_UNICODE_GENERALCATEGORY