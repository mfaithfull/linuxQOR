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

#ifndef QOR_PP_H_SIGNALS_PENDINGSLOT
#define QOR_PP_H_SIGNALS_PENDINGSLOT

#include "src/platform/compiler/compiler.h"
#include "src/qor/essentials/datastructures/rcuguarded.h"
#include "src/qor/essentials/datastructures/rculist.h"

namespace qor{

    class SignalBase;
    class qor_pp_module_interface(QOR_SIGNALS) SlotBase;
    enum class ConnectionKind;

    namespace detail 
    {
        class BentoAbstract;
        class TeaCupAbstract;
    }//detail

    class qor_pp_module_interface(QOR_SIGNALS) PendingSlot
    {
    public:
        
        ~PendingSlot();
        PendingSlot(const PendingSlot&) = delete;
        qor_pp_noinline PendingSlot(PendingSlot&&);

        PendingSlot(SignalBase* sender, std::unique_ptr<detail::BentoAbstract/*, qor_std_deleter<detail::BentoAbstract>*/ > signal_Bento, SlotBase* receiver,
            std::unique_ptr<detail::BentoAbstract/*, qor_std_deleter<detail::BentoAbstract>*/>  slot_Bento,
            std::unique_ptr<detail::TeaCupAbstract/*, qor_std_deleter<detail::TeaCupAbstract>*/ > teaCup_Data);

        SignalBase* sender() const; 
        SlotBase* receiver() const;
        std::unique_ptr<detail::BentoAbstract> internal_moveSlotBento();
        std::unique_ptr<detail::TeaCupAbstract> internal_moveTeaCup();
        void operator()() const;

    private:

        SignalBase* m_sender;
        std::unique_ptr<detail::BentoAbstract/*, qor_std_deleter<detail::BentoAbstract>*/ >  m_signal_Bento;
        SlotBase* m_receiver;
        std::unique_ptr<detail::BentoAbstract/*, qor_std_deleter<detail::BentoAbstract>*/ >  m_slot_Bento;
        std::unique_ptr<detail::TeaCupAbstract/*, qor_std_deleter<detail::TeaCupAbstract>*/ > m_teaCup_Data;
    };
    
}//qor

#endif//QOR_PP_H_SIGNALS_PENDINGSLOT
