// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_COLLECTIONADAPTER
#define QOR_PP_H_FRAMEWORK_DIO_COLLECTIONADAPTER

#include "valueadapter.h"
#include "collectionadapters/readcollection.h"
#include "collectionadapters/writecollection.h"
#include "collectionadapters/append.h"
#include "collectionadapters/collectiontostring.h"
#include "collectionadapters/streamincollection.h"
#include "collectionadapters/streamoutcollection.h"

namespace qor{ namespace dio{

    template<class range_t, typename... Bases >
    struct CollectionAdapter : public Bases...
    {
        using value_type = range_t::value_type;
        using collection_type = range_t;

        CollectionAdapter(range_t& t) : Bases(std::forward<range_t&>(t))...
        { }        

    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_COLLECTIONADAPTER
