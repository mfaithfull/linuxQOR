// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "textfilter.h"

namespace qor{ namespace text { namespace components {


    UnicodeTextFilter::UnicodeTextFilter() : pipeline::InlineProcessor<uint32_t, byte>(6, 6)
    { }

    UnicodeTextFilter::~UnicodeTextFilter() = default;

    void UnicodeTextFilter::Process(size_t& itemCount)
    {
        if(itemCount > 0)
        {
            byte* data = m_sourceBuffer.ReadRequest(itemCount);
            if(data)
            {
                uint32_t* space = reinterpret_cast<uint32_t*>(m_sinkBuffer.WriteRequest(itemCount));
                if(space)
                {
                    for(size_t index = 0; index < itemCount; ++index)
                    {
                        space[index] = static_cast<uint32_t>(data[index]);
                    }
                    m_sinkBuffer.WriteAcknowledge(itemCount);
                    m_sourceBuffer.ReadAcknowledge(itemCount);
                }
            }
        }
    }

}}}//qor::text::components