// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMIOCOLLECTION
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMIOCOLLECTION

#include "streamincollection.h"
#include "streamoutcollection.h"

namespace qor{ namespace dio{

    template<class range_t>
    struct StreamIOCollection : public StreamOutCollection<range_t>, public StreamInCollection<range_t>
    {
        StreamIOCollection(range_t& r) : StreamOutCollection<range_t>(r), StreamInCollection<range_t>(r)
        { }
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMIOCOLLECTION
