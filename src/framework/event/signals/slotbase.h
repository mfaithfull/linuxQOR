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

#ifndef QOR_PP_H_SIGNALS_SLOTBASE
#define QOR_PP_H_SIGNALS_SLOTBASE

#include <set>

#include "src/platform/compiler/compiler.h"
#include "src/qor/essentials/datastructures/rcuguarded.h"
#include "src/qor/essentials/datastructures/rculist.h"

namespace qor{

    class SignalBase;
    class PendingSlot;
    enum class ConnectionKind;

    namespace detail
    {
        class BentoAbstract;
    }//detail

    class qor_pp_module_interface(QOR_SIGNALS) SlotBase
    {
    public:

        SlotBase();
        SlotBase(const SlotBase&);
        virtual ~SlotBase() noexcept(false);

        SignalBase* sender() const;

    protected:

        std::set<SignalBase*> internal_senderList() const;

    private:

        static SignalBase*& get_threadLocal_currentSender();

        // list of possible Senders for this Receiver
        mutable SharedList<const SignalBase*> m_possibleSenders;

        virtual bool compareThreads() const;
        virtual void queueSlot(PendingSlot data, ConnectionKind type);
        friend class SignalBase;
        template<class Sender, class SignalClass, class ...SignalArgTypes, class ...Ts>
        friend void activate(Sender& sender, void (SignalClass::* signal)(SignalArgTypes...), Ts &&... Vs);
        template<class Sender, class Receiver>
        friend bool internal_disconnect(const Sender& sender, const detail::BentoAbstract* signalBento, const Receiver* receiver, const detail::BentoAbstract* slotBento);
    };

}//qor

#endif//QOR_PP_H_SIGNALS_SLOTBASE