// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <stdio.h>
#include "stringsource.h"
#include "src/framework/data/pipeline/sink.h"

namespace qor{ namespace pipeline{ namespace components{ 

    StringSource::StringSource()
    {
        m_data = "";
        m_it = m_data.begin();
    }

    StringSource::~StringSource() = default;

    void StringSource::SetData(const std::string& data)
    {
        m_data = data;
        m_it = m_data.begin();
    }

    size_t StringSource::ReadBytes(byte* space, size_t bytesToRead)
    {
        size_t bytesRead = 0;
        while(m_it != m_data.end() && bytesRead < bytesToRead)
        {
            space[bytesRead++] = *m_it++;
        }
        return bytesRead;
    }

}}}//qor::pipeline::components
