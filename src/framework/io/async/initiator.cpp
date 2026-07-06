// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "initiator.h"
#include "src/qor/flyers/error/commonerrormessages.h"

namespace qor { namespace io{ namespace async{

    Initiator::Initiator() { };
    Initiator::~Initiator() noexcept { };

    void Initiator::ConnectToProcessor(EventProcessor* /*processor*/){ }

    bool Initiator::RequiresBackgroundProcessor(){ return true; }

    IOTask Initiator::Bind(io::Descriptor* /*ioDescriptor*/, const io::network::Address& /*Address*/)
    {
        qor_pp_ofcontext;
        fatal(Error_empty_base_called);
        co_return Result{ .status_code = -1, .ioObject = nullptr};
    }

    IOTask Initiator::Listen(io::Descriptor* /*ioDescriptor*/, int /*backlog*/)
    {
        qor_pp_ofcontext;
        fatal(Error_empty_base_called);
        co_return Result{ .status_code = -1, .ioObject = nullptr};
    }

    IOTask Initiator::Accept(io::Descriptor* /*ioDescriptor*/, const io::network::Address& /*Address*/, io::network::Socket* /*Socket*/)
    {
        qor_pp_ofcontext;
        fatal(Error_empty_base_called);
        co_return Result{ .status_code = -1, .ioObject = nullptr};
    }

    IOTask Initiator::Read(io::Descriptor* /*ioDescriptor*/, byte* /*buffer*/, size_t /*len*/, long /*offset*/)
    {
        qor_pp_ofcontext;
        fatal(Error_empty_base_called);
        co_return Result{ .status_code = -1, .ioObject = nullptr};
    }

    IOTask Initiator::Write(io::Descriptor* /*ioDescriptor*/, byte* /*buffer*/, size_t /*len*/, long /*offset*/)
    {
        qor_pp_ofcontext;
        fatal(Error_empty_base_called);
        co_return Result{ .status_code = -1, .ioObject = nullptr};
    }

    IOTask Initiator::Send(io::Descriptor* /*ioDescriptor*/, const byte* /*buffer*/, size_t /*len*/, int /*flags*/)
    {
        qor_pp_ofcontext;
        fatal(Error_empty_base_called);
        co_return Result{ .status_code = -1, .ioObject = nullptr};
    }

    IOTask Initiator::Recv(io::Descriptor* /*ioDescriptor*/, byte* /*buffer*/, size_t /*len*/)
    {
        qor_pp_ofcontext;
        fatal(Error_empty_base_called);
        co_return Result{ .status_code = -1, .ioObject = nullptr};
    }

    IOTask Initiator::Shutdown(io::Descriptor* /*ioDescriptor*/, int /*how*/)
    {
        qor_pp_ofcontext;
        fatal(Error_empty_base_called);
        co_return Result{ .status_code = -1, .ioObject = nullptr};
    }

}}}//qor::io::async