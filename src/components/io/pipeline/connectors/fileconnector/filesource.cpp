// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "filesource.h"
#include "src/qor/flyers/error/error.h"

namespace qor{ namespace io{ namespace components{ 

    FileSource::FileSource() = default;

    FileSource::~FileSource() = default;

    size_t FileSource::ReadBytes(byte* space, size_t bytesToRead)
    {
        size_t result = 0;
        if(m_Connector && m_Connector->IsConnected())
        {
            result = m_Connector->File()->Read(space, bytesToRead, -1);

            if(result < 0)
            {
                OnReadError(result,0);
            }
            else if(result == 0)
            {
                OnEndOfData();
            }
        }
        else
        {
            continuable("Attempting to read from closed file.");
        }
        return result;
    }

}}}//qor::io::components
