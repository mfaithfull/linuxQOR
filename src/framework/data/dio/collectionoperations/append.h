// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_COLLECTIONADAPTERS_APPEND
#define QOR_PP_H_FRAMEWORK_DIO_COLLECTIONADAPTERS_APPEND

namespace qor{ namespace dio{

    template<class range_t>
    struct Append
    {
        Append(range_t& r) : m_(r)
        { }

        void push_back(const range_t::value_type t)
        {
            m_.push_back(t);
        }    

    private:
        range_t& m_;
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_COLLECTIONADAPTERS_APPEND
