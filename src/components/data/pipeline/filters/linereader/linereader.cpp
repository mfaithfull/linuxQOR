// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "linereader.h"

namespace qor{ namespace pipeline { namespace components{ 

    LineReader::LineReader() = default;
    LineReader::~LineReader() = default;
    LineReader& LineReader::operator = (const LineReader&) = default;

    bool LineReader::ReadFilter(size_t& unitsProcessed, size_t unitsToProcess)
    {
        return ReadUpToLineEnd(unitsProcessed, unitsToProcess);
    }

    bool LineReader::WriteFilter(size_t& unitsProcessed, size_t unitsToProcess)
    {
        return ReadUpToLineEnd(unitsProcessed, unitsToProcess);
    }

    bool LineReader::ReadUpToLineEnd(size_t& unitsProcessed, size_t unitsToProcess)
    {
        auto sourceBuffer = ActualSource()->GetBuffer();
        auto sinkBuffer = ActualSink()->GetBuffer();
        unitsProcessed = 0;
        if(sourceBuffer && sinkBuffer && unitsToProcess)
        {
            byte* data = sourceBuffer->ReadRequest(unitsToProcess);

            if(unitsToProcess > 0)
            {
                byte* scan = data;
                while(scan < (data + unitsToProcess - 1) && *scan != 0x0D && *scan != 0x0A)
                {
                    scan++;
                }

                size_t output_space = (scan - data);

                if(output_space > 0)
                {
                    if(*scan == 0x0D)
                    {
                        scan++; //skip the CR
                    }

                    if(*scan == 0x0A)
                    {
                        scan++; //skip the LF
                    }

                    size_t input_count = (scan - data);
                    byte* space = sinkBuffer->WriteRequest(output_space);
                    memcpy(space, data, output_space);
                    sinkBuffer->WriteAcknowledge(output_space);
                    sourceBuffer->ReadAcknowledge(input_count);
                    unitsProcessed = output_space;
                }
            }
            return true;
        }
        return false;
    }

}}}//qor::pipeline::components