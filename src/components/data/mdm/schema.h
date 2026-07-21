// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_DATA_MDM_SCHEMA
#define QOR_PP_H_COMPONENTS_DATA_MDM_SCHEMA

#include "src/platform/compiler/compiler.h"

namespace qor { namespace components { namespace data { namespace mdm {

    /*
        An indexed list of Lists
        An indexed list of Groups
        An indexed list of Tables
    */
    class Schema
    {

    };

    /*
    Schema should be persistable to and from JSON, with the list in seperate files
    Use the Schema name for the folder to put everything in, including the data.
    */
}}}}//qor::components::data::mdm

#endif//QOR_PP_H_COMPONENTS_DATA_MDM_SCHEMA
