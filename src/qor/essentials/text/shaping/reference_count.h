// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_SHAPING_REFERENCECOUNT
#define QOR_PP_H_TEXT_SHAPING_REFERENCECOUNT

#include "atomic.h"

namespace qor{ namespace qb{

    class reference_count_t
    {
    public:

        mutable atomic_t<int> ref_count;

        void init (int v = 1) { ref_count = v; }
        int get_relaxed () const { return ref_count; }
        int inc () const { return ref_count.inc (); }
        int dec () const { return ref_count.dec (); }
        void fini () { ref_count = -0x0000DEAD; }

        bool is_inert () const { return !ref_count; }
        bool is_valid () const { return ref_count > 0; }

    };

}}

#endif//QOR_PP_H_TEXT_SHAPING_REFERENCECOUNT