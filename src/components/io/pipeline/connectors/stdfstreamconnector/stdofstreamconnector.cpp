// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "stdofstreamconnector.h"

namespace qor{ namespace io{ namespace components{ 

    stdofstreamConnector::stdofstreamConnector() = default;

    stdofstreamConnector::stdofstreamConnector(io::filesystem::Index& fileIndex) : stdfstreamConnector(fileIndex){ }

    stdofstreamConnector::~stdofstreamConnector()
    {
        if(m_connected)
        {
            Disconnect();
        }
    }

    bool stdofstreamConnector::Connect()
    {
        m_ofstream.open(m_filePath, std::ios_base::out | std::ios_base::binary);
        m_connected = m_ofstream.is_open() && !m_ofstream.fail();
        return stdfstreamConnector::Connect();
    }

    void stdofstreamConnector::Disconnect()
    {
        m_ofstream.close();
        stdfstreamConnector::Disconnect();
    }

    std::ofstream& stdofstreamConnector::Stream()
    {
        return m_ofstream;
    }
    
}}}//qor::io::components
