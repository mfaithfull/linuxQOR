// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SYNC_MUTEX
#define QOR_PP_H_SYNC_MUTEX

#include <mutex>
#include "syncobject.h"

namespace qor{

    class qor_pp_module_interface_gcc(QOR_SYNC) Mutex : public SyncObject
    {
    public:

        qor_pp_module_interface(QOR_SYNC) Mutex() noexcept;
        qor_pp_module_interface(QOR_SYNC) virtual ~Mutex();

        qor_pp_module_interface(QOR_SYNC) virtual void Acquire();
        qor_pp_module_interface(QOR_SYNC) virtual void Release();
        qor_pp_module_interface(QOR_SYNC) virtual bool IsLocked() const;

    private:

        bool m_Locked{ false };
        std::mutex m_Impl;
    };

}//qor

#endif//QOR_PP_H_SYNC_MUTEX
