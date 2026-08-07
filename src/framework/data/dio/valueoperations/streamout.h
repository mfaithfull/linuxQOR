// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMOUT
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMOUT

#include <iostream>

namespace qor{ namespace dio{

    template<typename T>
    struct StreamOut
    {
        StreamOut(T& t) : m_(t)
        { }

        template<class stream_t>
        inline stream_t& operator << (stream_t& s) const
        {
            s << m_;
            return s;
        }

    private:
        T& m_;
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMOUT
