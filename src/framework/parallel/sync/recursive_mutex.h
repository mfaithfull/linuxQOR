// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SYNC_RECURSIVEMUTEX
#define QOR_PP_H_SYNC_RECURSIVEMUTEX

#include <mutex>
#include "syncobject.h"

namespace qor{

    class qor_pp_module_interface(QOR_SYNC) RecursiveMutex : public SyncObject
    {
    public:

        RecursiveMutex();
        virtual ~RecursiveMutex();

        virtual void Acquire();
        virtual void Release();
        virtual bool IsLocked() const;

    private:
        size_t m_Locked;
        std::recursive_mutex m_Impl;
    };

}//qor

#endif//QOR_PP_H_SYNC_RECURSIVEMUTEX
