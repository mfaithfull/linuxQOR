// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

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
#include "src/qor/datastructures/rcuguarded.h"
#include "src/qor/datastructures/rculist.h"

namespace qor{

    class SignalBase;
    class SlotBase;
    enum class ConnectionKind;

    namespace detail 
    {
        class BentoAbstract;
        class TeaCupAbstract;
    }

    class qor_pp_module_interface(QOR_SIGNALSLOTS) PendingSlot
    {
    public:

        PendingSlot(const PendingSlot&) = delete;
        PendingSlot(PendingSlot&&) = default;

        PendingSlot(SignalBase* sender, std::unique_ptr<detail::BentoAbstract/*, qor_std_deleter<detail::BentoAbstract>*/ > signal_Bento, SlotBase* receiver,
            std::unique_ptr<detail::BentoAbstract/*, qor_std_deleter<detail::BentoAbstract>*/>  slot_Bento,
            std::unique_ptr<detail::TeaCupAbstract/*, qor_std_deleter<detail::TeaCupAbstract>*/ > teaCup_Data);

        SignalBase* sender() const 
        {
            return m_sender;
        }

        SlotBase* receiver() const 
        {
            return m_receiver;
        }

        std::unique_ptr<detail::BentoAbstract/*, qor_std_deleter<detail::BentoAbstract>*/ > internal_moveSlotBento()
        {
            return std::move(m_slot_Bento);
        }

        std::unique_ptr<detail::TeaCupAbstract/*, qor_std_deleter<detail::TeaCupAbstract>*/ > internal_moveTeaCup()
        {
            return std::move(m_teaCup_Data);
        }

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
