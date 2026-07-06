// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "base64encodefilter.h"

namespace qor{ namespace pipeline { namespace components{ 

    Base64EncodeFilter::Base64EncodeFilter() : 
            qor::pipeline::InlineFilter<qor::byte>(0)
    { }
    
    Base64EncodeFilter::Base64EncodeFilter(size_t itemCount) : 
            qor::pipeline::InlineFilter<qor::byte>(itemCount)
    { }

    Base64EncodeFilter::~Base64EncodeFilter() = default;

    void Base64EncodeFilter::Filter(qor::byte* space, qor::byte* data, size_t& unitsToProcess, size_t& writeCount)
    {
        static const char* base64_chars = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

        if(unitsToProcess)
        {
            size_t output_space = (unitsToProcess + 2 ) / 3 * 4;
            output_space = std::min(output_space, writeCount);
            int val = 0, valb = -6;
            size_t index = 0;
            size_t outindex = 0;
            while(index < unitsToProcess && outindex < output_space)
            {
                val = (val << 8) + data[index++];
                valb += 8;
                while(valb >= 0 && outindex < output_space)
                {
                    byte output = base64_chars[(val >> valb) & 0x3f];
                    space[outindex++] = output;
                    valb -= 6;
                }
            };
            if (valb > -6 && outindex < output_space) 
            {
                byte output = base64_chars[((val << 8) >> valb) & 0x3F];
                space[outindex++] = output;
            }
            while (outindex % 4 && outindex < output_space)
            {
                space[outindex++] = '=';
            }
            unitsToProcess = index;
            writeCount = outindex;            
        }
    }
}}}//qor::pipeline::components