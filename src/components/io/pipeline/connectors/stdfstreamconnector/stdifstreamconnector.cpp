// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "stdifstreamconnector.h"

namespace qor{ namespace io { namespace components{ 

    stdifstreamConnector::stdifstreamConnector() = default;

    stdifstreamConnector::stdifstreamConnector(filesystem::Index& fileIndex) : stdfstreamConnector(fileIndex){ }

    stdifstreamConnector::~stdifstreamConnector()
    {
        if(m_connected)
        {
            Disconnect();
        }
    }

    bool stdifstreamConnector::Connect()
    {
        m_ifstream.open(m_filePath, std::ios_base::in | std::ios_base::binary);
        m_connected = m_ifstream.is_open() && !m_ifstream.fail();
        return stdfstreamConnector::Connect();
    }

    void stdifstreamConnector::Disconnect()
    {
        m_ifstream.close();
        stdfstreamConnector::Disconnect();
    }

    std::ifstream& stdifstreamConnector::Stream()
    {
        return m_ifstream;
    }
    
}}}//qor::io::components
