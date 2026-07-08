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

#include <mutex>

#include "do_signal.h"
#include "slotbase.h"
#include "detail/bento.h"

namespace qor
{
    SignalBase::~SignalBase() noexcept(false)
    {
        try 
        {
            auto senderListHandle = m_connectList.lock_read();

            if (m_activateBusy > 0) 
            {
                // activate() called a slot which then destroys this sender
                std::lock_guard<std::mutex> lock(get_mutex_beingDestroyed());
                get_beingDestroyed().insert(this);
            }

            for (auto& item : *senderListHandle) 
            {
                const SlotBase* receiver = item.receiver;
                if (receiver != nullptr) 
                {
                    auto receiverListHandle = receiver->m_possibleSenders.lock_write();
                    auto iter = receiverListHandle->begin();

                    while (iter != receiverListHandle->end()) 
                    {
                        if (*iter == this)
                        {
                            iter = receiverListHandle->erase(iter);
                        }
                        else {
                            ++iter;
                        }

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

    detail::BentoAbstract*& SignalBase::get_threadLocal_currentSignal()
    {

#ifdef __APPLE__
        static __thread detail::BentoAbstract* threadLocal_currentSignal = nullptr;
#else
        static qor_pp_thread_local detail::BentoAbstract* threadLocal_currentSignal = nullptr;
#endif

        return threadLocal_currentSignal;
    }

    std::mutex& SignalBase::get_mutex_beingDestroyed()
    {
        static std::mutex mutex_beingDestroyed;

        return mutex_beingDestroyed;
    }

    std::unordered_set<const SignalBase*>& SignalBase::get_beingDestroyed()
    {
        static std::unordered_set<const SignalBase*> beingDestroyed;

        return beingDestroyed;
    }

    void SignalBase::addConnection(std::unique_ptr<const detail::BentoAbstract/*, qor_std_deleter<const detail::BentoAbstract>*/> signalMethod, const SlotBase* receiver, std::unique_ptr<const detail::BentoAbstract/*, qor_std_deleter<const detail::BentoAbstract>*/> slotMethod, ConnectionKind type, SharedList<ConnectStruct>::write_handle senderListHandle) const
    {
        struct ConnectStruct tempStruct;

        tempStruct.signalMethod = std::move(signalMethod);
        tempStruct.receiver = receiver;
        tempStruct.slotMethod = std::move(slotMethod);
        tempStruct.type = type;

        senderListHandle->push_back(std::move(tempStruct));

        // broom - senderListHandle->unlock()

        if (receiver != nullptr) 
        {
            auto receiverListHandle = receiver->m_possibleSenders.lock_write();
            receiverListHandle->push_back(this);
        }
    }

    void SignalBase::handleException(std::exception_ptr)
    {
    }

    int SignalBase::internal_cntConnections(const SlotBase* receiver, const detail::BentoAbstract& signalMethod_Bento) const
    {
        int retval = 0;

        auto senderListHandle = m_connectList.lock_read();

        for (auto& item : *senderListHandle) 
        {
            if (receiver && item.receiver != receiver) 
            {
                continue;
            }

            if (*(item.signalMethod) != signalMethod_Bento) 
            {
                continue;
            }

            retval++;
        }
        return retval;
    }

    std::set<SlotBase*> SignalBase::internal_receiverList( const detail::BentoAbstract& signalMethod_Bento) const
    {
        std::set<SlotBase*> retval;
        auto senderListHandle = m_connectList.lock_read();
        for (auto& item : *senderListHandle) 
        {
            if (*(item.signalMethod) != signalMethod_Bento) 
            {
                continue;
            }
            retval.insert(const_cast<SlotBase*>(item.receiver));
        }
        return retval;
    }

}//qor
