// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "pipesink.h"
#include "src/framework/data/pipeline/source.h"

namespace qor{ namespace io{ namespace components{

    PipeSink::PipeSink() = default;

    PipeSink::~PipeSink() = default;

    size_t PipeSink::WriteBytes(byte* data, size_t bytesToWrite)
    {
        size_t result = 0;
        if(m_Connector && m_Connector->IsConnected())
        {
            result = m_Connector->Pipe()->Write((byte*)data, bytesToWrite);
        }
        else
        {
            OnWriteError(0,0);
        }
        return result;
    }

}}}//qor::io::components
