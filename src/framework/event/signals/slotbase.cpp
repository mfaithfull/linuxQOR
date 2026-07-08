// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

/***********************************************************************
*
* Copyright (c) 2016-2021 Barbara Geller
* Copyright (c) 2016-2021 Ansel Sermersheim
*
* This file is part of CsSignal.
*
* CsSignal is free software, released under the BSD 2-Clause license.
* For license details refer to LICENSE provided with this project.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* https://opensource.org/licenses/BSD-2-Clause
*
***********************************************************************/

#include "src/configuration/configuration.h"
#include "slotbase.h"
#include "pendingslot.h"
#include "signalbase.h"
#include "detail/bento.h"

namespace qor
{
    SlotBase::SlotBase(){}

    SlotBase::SlotBase(const SlotBase&){}

    SlotBase::~SlotBase() noexcept(false)
    {
        try 
        {
            // clean up possible sender connections
            auto receiverListHandle = m_possibleSenders.lock_read();

            for (auto& sender : *receiverListHandle) 
            {
                auto senderListHandle = sender->m_connectList.lock_write();

                auto iter = senderListHandle->begin();

                while (iter != senderListHandle->end()) 
                {
                    if (iter->receiver == this) 
                    {
                        iter = senderListHandle->erase(iter);
                    }
                    else 
                    {
                        ++iter;
                    }
                }
            }

        }
        catch (...) 
        {
            // least of the worst options
            std::terminate();
        }
    }
    
    SignalBase*& SlotBase::get_threadLocal_currentSender()
    {
#ifdef __APPLE__
        static __thread SignalBase* threadLocal_currentSender = nullptr;
#else
        static qor_pp_thread_local SignalBase* threadLocal_currentSender = nullptr;
#endif

        return threadLocal_currentSender;
    }

    bool SlotBase::compareThreads() const
    {
        return true;
    }

    void SlotBase::queueSlot(PendingSlot data, ConnectionKind)
    {
        // calls the slot immediately
        data();
    }

    SignalBase* SlotBase::sender() const
    {
        return get_threadLocal_currentSender();
    }

    std::set<SignalBase*> SlotBase::internal_senderList() const
    {
        std::set<SignalBase*> retval;

        auto receiverListHandle = m_possibleSenders.lock_read();

        for (auto& sender : *receiverListHandle) 
        {
            retval.insert(const_cast<SignalBase*>(sender));
        }

        return retval;
    }

}//qor
