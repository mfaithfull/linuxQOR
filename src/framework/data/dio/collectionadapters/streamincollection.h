// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMINCOLLECTION
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMINCOLLECTION

#include <iostream>

namespace qor{ namespace dio{

    template<class range_t>
    struct StreamInCollection
    {
        StreamInCollection(range_t& r) : m_(r)
        { }

        template<class stream_t>
        inline stream_t& operator >> (stream_t& s) const
        {
            typename range_t::value_type t;
            s >> t;
            m_.push_back(t);
            return s;
        }

    private:
        range_t& m_;
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMINCOLLECTION
