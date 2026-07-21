// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_DATA_MDM_TABLE
#define QOR_PP_H_COMPONENTS_DATA_MDM_TABLE

#include "src/platform/compiler/compiler.h"

namespace qor { namespace components { namespace data { namespace mdm {

    //A named ordered list of list index and maybe something to describe the record type
    class Table
    {
        //File
    };

    /*
        Each Table definition can have a corresponding file for the actual data
        It will also need all the list and group definitions from the schema to be useful
        Load in the JSON and build the in memory table from it along with any indexes for direct access.
        Persist the data back out to JSON or to a more efficient binary format
    */
}}}}//qor::components::data::mdm

#endif//QOR_PP_H_COMPONENTS_DATA_MDM_TABLE