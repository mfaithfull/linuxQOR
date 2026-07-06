// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "base64decodefilter.h"

namespace qor{ namespace pipeline{ namespace components{ 

    Base64DecodeFilter::Base64DecodeFilter() = default;
    Base64DecodeFilter::~Base64DecodeFilter() = default;

    bool Base64DecodeFilter::ReadFilter(size_t& unitsProcessed, size_t unitsToProcess)
    {
        return Decode(unitsProcessed, unitsToProcess);
    }

    bool Base64DecodeFilter::WriteFilter(size_t& unitsProcessed, size_t unitsToProcess)
    {
        return Decode(unitsProcessed, unitsToProcess);
    }

    static unsigned int pos_of_char(const unsigned char chr) 
    {
        if      (chr >= 'A' && chr <= 'Z') return chr - 'A';
        else if (chr >= 'a' && chr <= 'z') return chr - 'a' + ('Z' - 'A')               + 1;
        else if (chr >= '0' && chr <= '9') return chr - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
        else if (chr == '+' || chr == '-') return 62; // Be liberal with input and accept both url ('-') and non-url ('+') base 64 characters (
        else if (chr == '/' || chr == '_') return 63; // Ditto for '/' and '_'
        else
        {
            //TODO: Error not base64encoded
        }
        return 0;
    }

    bool Base64DecodeFilter::Decode(size_t& unitsProcessed, size_t unitsToProcess)
    {
        auto sourceBuffer = ActualSource()->GetBuffer();
        auto sinkBuffer = ActualSink()->GetBuffer();
        if(sourceBuffer && sinkBuffer && unitsToProcess)
        {
            byte* data = sourceBuffer->ReadRequest(unitsToProcess);
            size_t output_space = unitsToProcess / 4 * 3;
            byte* space = sinkBuffer->WriteRequest(output_space);
            size_t index = 0;
            size_t outindex = 0;
            while( index < (unitsToProcess-1) && outindex < output_space)
            {
                size_t pos_of_char_1 = pos_of_char(data[index + 1] );
                byte output = static_cast<byte>((((pos_of_char(data[index])) << 2) + ((pos_of_char_1 & 0x30) >> 4)));
                space[outindex++] = output;

                if(( index + 2 < unitsToProcess) &&                    // Check for data that is not padded with equal signs (which is allowed by RFC 2045)
                        data[index+2]!= '=' && data[index+2] != '.')   // accept URL-safe base 64 strings, too, so check for '.' also.
                {
                    unsigned int pos_of_char_2 = pos_of_char(data[index + 2]);
                    output = ((( pos_of_char_1 & 0x0f) << 4) + ((pos_of_char_2 & 0x3c) >> 2));
                    space[outindex++] = output;

                    if((index + 3 < unitsToProcess) && data[index + 3] != '=' && data[index + 3] != '.')
                    {
                        output = (((pos_of_char_2 & 0x03) << 6) + pos_of_char(data[index+3]));
                        space[outindex++] = output;
                    }
                }
                index += 4;
            }
            sinkBuffer->WriteAcknowledge(outindex);
            sourceBuffer->ReadAcknowledge(index);
            unitsProcessed = outindex;
            return true;
        }
        return false;
    }

}}}//qor::pipeline::components