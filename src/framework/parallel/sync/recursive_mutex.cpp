// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "recursive_mutex.h"

namespace qor{

    RecursiveMutex::RecursiveMutex(): m_Locked {0}{ }
    RecursiveMutex::~RecursiveMutex(){ }//required to supress autoexporting of =default version by Microsoft Linker

    void RecursiveMutex::Acquire()
    {        
        m_Impl.lock();
        m_Locked++;
    }

    void RecursiveMutex::Release()
    {   
        m_Locked--;
        m_Impl.unlock();        
    }
    
    bool RecursiveMutex::IsLocked() const
    {
        return m_Locked > 0 ? true : false;
    }
        
}//qor
