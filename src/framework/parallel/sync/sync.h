// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SYNC
#define QOR_PP_H_SYNC

#include "nullsection.h"

//this trait attaches a synchronisation type to the type T
//Shared references to T will use this type to protect access

namespace qor{

    template<typename T>
    struct sync_of
    {
        typedef NullSection type;
    };

}//qor

//Preprocessor macro shorthand for declaring a sync_of specialisation
#   define qor_pp_declare_sync_of(_CLASS,_SYNC)\
    template<> struct sync_of< _CLASS >\
    {\
        typedef _SYNC type;\
    };

//Example: qor_pp_declare_sync_of(SharedDataHolder, Mutex);

#endif//QOR_PP_H_SYNC
