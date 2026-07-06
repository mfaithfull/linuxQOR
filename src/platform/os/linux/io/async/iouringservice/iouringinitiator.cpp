// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "iouringinitiator.h"
#include "iouringeventprocessor.h"

qor_pp_module_provide(QOR_LINUXASYNCIOSERVICE, Initiator)

namespace qor{ namespace io{ namespace async{ namespace lin{

    void IOUringInitiator::ConnectToProcessor(qor::io::async::EventProcessor* processor)
    {
        IOUringEventProcessor* eventProcessor = dynamic_cast<IOUringEventProcessor*>(processor);
        if(eventProcessor)
        {
            m_Ring = const_cast<IOUring*>(&(eventProcessor->Ring()));//The ring initiator must have the ring
        }
    }

}}}}//qor::io::async::lin
