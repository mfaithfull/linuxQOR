// Copyright Querysoft Limited 2008 - 2025

#include "src/configuration/configuration.h"
#include "ucdfunctions.h"
#include "ucdtable.h"

namespace qor{ namespace unicode {

    static int _cmp_pair (const void *_key, const void *_item)
    {
        uint64_t& a = * (uint64_t*) _key;
        uint64_t b = (* (uint64_t*) _item) & encode3(0x1FFFFFu, 0x1FFFFFu, 0);

        return a < b ? -1 : a > b ? +1 : 0;
    }

    static int _cmp_pair_11_7_14 (const void *_key, const void *_item)
    {
        uint32_t& a = * (uint32_t*) _key;
        uint32_t b = (* (uint32_t*) _item) & encode3_11_7_14(0x1FFFFFu, 0x1FFFFFu, 0);

        return a < b ? -1 : a > b ? +1 : 0;
    }
/*
    combining_class_t UCDFunctions::combining_class(codepoint_t cp)
    {
        return (combining_class_t) ucd_ccc(cp);
    }

    unsigned int UCDFunctions::eastasian_width(codepoint_t cp)
    {
        return 1;
    }*/

    general_category_t UCDFunctions::general_category(codepoint_t cp)
    {
        return (general_category_t) ucd_gc(cp);
    }

    codepoint_t UCDFunctions::mirroring(codepoint_t cp)
    {
        return cp + ucd_bmg(cp);
    }

    enum script UCDFunctions::script(codepoint_t cp)
    {
        return ucd_sc_map[ucd_sc(cp)];
    }

    bool UCDFunctions::compose(codepoint_t a, codepoint_t b, codepoint_t* ab)
    {
        // Hangul is handled algorithmically.
        if (compose_hangul(a, b, ab)) return true;

        codepoint_t u = 0;

        if ((a & 0xFFFFF800u) == 0x0000u && (b & 0xFFFFFF80) == 0x0300u)
        {
            // If "a" is small enough and "b" is in the U+0300 range, the composition data is encoded in a 32bit array sorted by "a,b" pair.
            uint32_t k = encode3_11_7_14(a, b, 0);
            const uint32_t *v = qb::hb_bsearch (k, ucd_dm2_u32_map, qb::ARRAY_LENGTH(ucd_dm2_u32_map), sizeof (*ucd_dm2_u32_map), _cmp_pair_11_7_14);
            if(!v) return false;
            u = decode3_11_7_14_3(*v);
        }
        else
        {
            // Otherwise it is stored in a 64bit array sorted by "a,b" pair.
            uint64_t k = encode3(a, b, 0);
            const uint64_t *v = qb::hb_bsearch(k, ucd_dm2_u64_map, qb::ARRAY_LENGTH(ucd_dm2_u64_map), sizeof (*ucd_dm2_u64_map), _cmp_pair);
            if(!v) return false;
            u = decode3_3(*v);
        }

        if(!u) return false;
        *ab = u;
        return true;
    }

    bool UCDFunctions::decompose(codepoint_t ab, codepoint_t* a, codepoint_t* b)
    {
        if (decompose_hangul(ab, a, b)) return true;

        unsigned i = ucd_dm(ab);

        // If no data, there's no decomposition.
        if (!i) return false;
        i--;

        //Check if it's a single-character decomposition.
        if( i < qb::ARRAY_LENGTH(ucd_dm1_p0_map) + qb::ARRAY_LENGTH(ucd_dm1_p2_map))
        {
            // Single-character decompositions currently are only in plane 0 or plane 2.
            if (i < qb::ARRAY_LENGTH(ucd_dm1_p0_map))
            {
                // Plane 0
                *a = ucd_dm1_p0_map[i];
            }
            else
            {
                // Plane 2.
                i -= qb::ARRAY_LENGTH(ucd_dm1_p0_map);
                *a = 0x20000 | ucd_dm1_p2_map[i];
            }
            *b = 0;
            return true;
        }

        i -= qb::ARRAY_LENGTH(ucd_dm1_p0_map) + qb::ARRAY_LENGTH(ucd_dm1_p2_map);

        // Otherwise they are encoded either in a 32bit array or a 64bit array.
        if(i < qb::ARRAY_LENGTH(ucd_dm2_u32_map))
        {
            // 32bit array.
            uint32_t v = ucd_dm2_u32_map[i];
            *a = decode3_11_7_14_1(v);
            *b = decode3_11_7_14_2(v);
            return true;
        }

        i -= qb::ARRAY_LENGTH(ucd_dm2_u32_map);

        // 64bit array.
        uint64_t v = ucd_dm2_u64_map[i];
        *a = decode3_1(v);
        *b = decode3_2(v);
        return true;
    }

    unsigned int UCDFunctions::decompose_compatability(codepoint_t u, codepoint_t* decomposed)
    {
        return 0;
    }


}}//qor::unicode
