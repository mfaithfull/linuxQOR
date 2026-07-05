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
#include "pendingslot.h"
#include "detail/bento.h"

namespace qor
{    
    PendingSlot::~PendingSlot() { }

    PendingSlot::PendingSlot(PendingSlot&&) = default;
    
    PendingSlot::PendingSlot(SignalBase* sender, std::unique_ptr<detail::BentoAbstract/*, qor_std_deleter<detail::BentoAbstract>*/ > signal_Bento,
        SlotBase* receiver, std::unique_ptr< detail::BentoAbstract/*, qor_std_deleter<detail::BentoAbstract>*/ > slot_Bento,
        std::unique_ptr< detail::TeaCupAbstract/*, qor_std_deleter<detail::TeaCupAbstract>*/ > teaCup_Data)
        : m_sender(sender), m_signal_Bento(std::move(signal_Bento)), m_receiver(receiver), m_slot_Bento(std::move(slot_Bento)), m_teaCup_Data(std::move(teaCup_Data))
    { }

    SignalBase* PendingSlot::sender() const 
    {
        return m_sender;
    }

    SlotBase* PendingSlot::receiver() const 
    {
        return m_receiver;
    }

    std::unique_ptr<detail::BentoAbstract/*, qor_std_deleter<detail::BentoAbstract>*/ > 
        PendingSlot::internal_moveSlotBento()
    {
        return std::move(m_slot_Bento);
    }

    std::unique_ptr<detail::TeaCupAbstract/*, qor_std_deleter<detail::TeaCupAbstract>*/ > 
        PendingSlot::internal_moveTeaCup()
    {
        return std::move(m_teaCup_Data);
    }

    void PendingSlot::operator()() const
    {
        // invoke the slot
        m_slot_Bento->invoke(m_receiver, m_teaCup_Data.get());
    }

}//qor
