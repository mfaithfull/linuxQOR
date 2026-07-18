// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "stringsink.h"
#include "src/framework/data/pipeline/source.h"

namespace qor{ namespace pipeline{ namespace components{ 

    StringSink::StringSink() = default;
    
    StringSink::~StringSink() = default;

    std::string StringSink::GetData()
    {
        return m_data;
    }
    
    size_t StringSink::WriteBytes(byte* data, size_t bytesToWrite)
    {
        m_data.append(std::string((const char*)(data), bytesToWrite));
        return bytesToWrite;
    }

}}}//qor::pipeline::components
