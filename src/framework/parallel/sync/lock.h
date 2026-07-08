// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SYNC_LOCK
#define QOR_PP_H_SYNC_LOCK

#include "syncobject.h"

namespace qor{
		
    class Lock final
    {
        friend class Key;

    public:

        inline Lock(SyncObject& SyncObject) : m_SyncObject(SyncObject)
        {
            m_SyncObject.Acquire();
        }

        Lock(const Lock&) = delete;
        Lock(Lock&&) = delete;

        inline ~Lock()
        {
            m_SyncObject.Release();
        }

    private:

        SyncObject& m_SyncObject;
    };

    class Key
    {
    public:

        inline Key(Lock& lock) : m_Lock(lock)
        {
            m_Lock.m_SyncObject.Release();
        }

        Key(const Key&) = delete;
        Key(Key&&) = delete;

        inline ~Key()
        {
            m_Lock.m_SyncObject.Acquire();
        }        

    private:

        Lock& m_Lock;
    };

}//qor

#endif//QOR_PP_H_SYNC_LOCK
