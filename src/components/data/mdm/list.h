// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_DATA_MDM_LIST
#define QOR_PP_H_COMPONENTS_DATA_MDM_LIST

#include "src/platform/compiler/compiler.h"

namespace qor { namespace components { namespace data { namespace mdm {

    //A named list of names, so it can be in a list of lists.
    //Automatically assigned indexes for names
    //A hash map of some kind to get the index back from a matching name
    //persistence to and from JSON
    //No reuse of indexes on removal or ever, they just become empty and can be filtered out.
    class List
    {
        /*
        ToJSON
        FromJSON        
        */
    };

}}}}//qor::components::data::mdm

#endif//QOR_PP_H_COMPONENTS_DATA_MDM_LIST