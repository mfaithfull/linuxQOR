// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "stdofstreamsink.h"
#include "src/framework/data/pipeline/source.h"

namespace qor{ namespace io{ namespace components{ 

    stdofstreamSink::stdofstreamSink() = default;
    stdofstreamSink::~stdofstreamSink() = default;

    size_t stdofstreamSink::WriteBytes(byte* data, size_t bytesToWrite)
    {
        size_t result = 0;
        if(m_Connector && m_Connector->IsConnected())
        {
            m_Connector->Stream().write(reinterpret_cast<std::ifstream::char_type*>(data), bytesToWrite);
            result = bytesToWrite;
        }
        return result;
    }

}}}//qor::io::components
