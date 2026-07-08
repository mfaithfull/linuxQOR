// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SYNC_NULLLOCK
#define QOR_PP_H_SYNC_NULLLOCK

#include "syncobject.h"

namespace qor{
		
    class NullLock
    {
    public:

        NullLock() = default;
        NullLock(SyncObject& SyncObject) {}
        ~NullLock() = default;
    };

}//qor

#endif//QOR_PP_H_SYNC_NULLLOCK
