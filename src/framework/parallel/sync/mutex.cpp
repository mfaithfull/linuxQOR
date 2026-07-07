// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "mutex.h"

namespace qor{

    Mutex::Mutex() noexcept = default;

    Mutex::~Mutex() = default;

    void Mutex::Acquire()
    {
        m_Impl.lock();
        m_Locked = true;
    }

    void Mutex::Release()
    {
        m_Locked = false;
        m_Impl.unlock();
    }
        
    //Note results are inherently unreliable in an actual mutithreaded environment. 
    //They only really make sense in a startup, single threaded situation.
    bool Mutex::IsLocked() const
    {
        return m_Locked;
    }

}//qor
