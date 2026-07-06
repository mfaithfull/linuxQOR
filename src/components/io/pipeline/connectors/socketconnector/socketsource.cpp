// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "socketsource.h"
#include "src/framework/data/pipeline/sink.h"
#include "src/qor/flyers/error/error.h"

namespace qor{ namespace io{ namespace network{ namespace components{ 

    SocketSource::SocketSource() = default;

    SocketSource::~SocketSource() = default;

    size_t SocketSource::ReadBytes(byte* space, size_t bytesToRead)
    {
        int32_t result = 0;
        if(m_Connector && m_Connector->IsConnected())
        {
            result = m_Connector->Receive((char*)space, (int32_t)bytesToRead, 0);
        }
        else
        {
            continuable("Attempting to read from disconnected socket.");
        }
        if(result == -1)
        {
            return 0;
        }
        return result;
    }

}}}}//qor::io::network::components
