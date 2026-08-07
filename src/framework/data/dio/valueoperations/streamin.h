// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMIN
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMIN

#include <iostream>

namespace qor{ namespace dio{

    template<typename T>
    struct StreamIn
    {
        StreamIn(T& t) : m_(t)
        { }

        template<class stream_t>
        inline stream_t& operator >> (stream_t& s)
        {
            s >> m_;
            return s;
        }

    private:
        T& m_;
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMIN
