// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "pipesource.h"
#include "src/qor/flyers/error/error.h"

namespace qor{ namespace io{ namespace components{

    PipeSource::PipeSource() = default;

    PipeSource::~PipeSource() = default;

    size_t PipeSource::ReadBytes(byte* space, size_t bytesToRead)
    {
        size_t result = 0;
        if(m_Connector && m_Connector->IsConnected())
        {
            result = m_Connector->Pipe()->Read(space, bytesToRead, -1);

            if(result == 0)
            {
                OnEndOfData();
            }
        }
        else
        {
            continuable("Attempting to read from closed pipe.");
        }
        return result;
    }

}}}//qor::io::components
