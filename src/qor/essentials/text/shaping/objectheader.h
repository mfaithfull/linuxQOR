// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_SHAPING_OBJECT_HEADER
#define QOR_PP_H_TEXT_SHAPING_OBJECT_HEADER

#include "reference_count.h"
#include "user_data_array.h"

namespace qor{ namespace qb{

    class object_header_t
    {
        reference_count_t ref_count;
        mutable atomic_t<bool> writable = false;
        atomic_t<user_data_array_t *> user_data;

        bool is_inert () const 
        { 
            return !ref_count.get_relaxed (); 
        }

    };

}}

#endif//QOR_PP_H_TEXT_SHAPING_OBJECT_HEADER
