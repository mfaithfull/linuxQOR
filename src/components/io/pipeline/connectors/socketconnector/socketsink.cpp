// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "socketsink.h"
#include "src/framework/data/pipeline/source.h"
#include "src/qor/flyers/error/error.h"

namespace qor{ namespace io { namespace network{ namespace components{ 

    SocketSink::SocketSink() = default;

    SocketSink::~SocketSink() = default;

    size_t SocketSink::WriteBytes(byte* data, size_t bytesToWrite)
    {
        size_t result = 0;
        if(m_Connector && m_Connector->IsConnected())
        {
            result = (int32_t)m_Connector->Send((const char*)data, (int32_t)bytesToWrite);
        }
        else
        {
            continuable("Attempting to write to disconnected socket.");
        }
        return result;
    }

}}}}//qor::io::network::components
