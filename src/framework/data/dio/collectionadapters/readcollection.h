// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_READCOLLECTION
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_READCOLLECTION

namespace qor{ namespace dio{

    template<typename range_t>
    struct ReadCollection
    {    
        ReadCollection(range_t& r) : m_(r)
        { }

        range_t Get() const {return m_;}

    private:
        range_t& m_;
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_READCOLLECTION
