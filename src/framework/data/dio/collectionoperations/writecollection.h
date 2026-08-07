// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_WRITECOLLECTION
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_WRITECOLLECTION

#include <vector>

namespace qor{ namespace dio{

    template<typename range_t>
    struct WriteCollection
    {
        WriteCollection(range_t& t) : m_(t)
        { }

        void Set(const range_t t)
        {
            m_ = t;
        }    

    private:
        range_t& m_;        
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_WRITECOLLECTION
