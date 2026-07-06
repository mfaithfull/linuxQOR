// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "stdfstreamconnector.h"

namespace qor{ namespace io{ namespace components{ 

    stdfstreamConnector::stdfstreamConnector() = default;
    
    stdfstreamConnector::stdfstreamConnector(io::filesystem::Index& fileIndex) : Plug()
    {
        SetFile(fileIndex);
    }

    stdfstreamConnector::~stdfstreamConnector() = default;

    void stdfstreamConnector::SetFile(io::filesystem::Index& fileIndex)
    {
        m_filePath = fileIndex.GetPath().ToString();
    }

    bool stdfstreamConnector::Connect()
    {
        return IsConnected();
    }

    void stdfstreamConnector::Disconnect()
    {
        m_connected = false;
    }
    
}}}//qor::io::components
