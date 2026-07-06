// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/framework/data/pipeline/copyfilter.h"

namespace qor{ namespace pipeline{ 

    bool CopyFilter::ReadFilter(size_t& unitsProcessed, size_t unitsToProcess)
    {
        return Copy(unitsProcessed, unitsToProcess);
    }

    bool CopyFilter::WriteFilter(size_t& unitsProcessed, size_t unitsToProcess)
    {
        return Copy(unitsProcessed, unitsToProcess);
    }

    bool CopyFilter::Copy(size_t& unitsProcessed, size_t unitsToProcess)
    {
        auto sourceBuffer = ActualSource()->GetBuffer();
        auto sinkBuffer = ActualSink()->GetBuffer();
        if(sourceBuffer && sinkBuffer && unitsToProcess)
        {
            byte* space = sinkBuffer->WriteRequest(unitsToProcess);
            byte* data = sourceBuffer->ReadRequest(unitsToProcess);
            Process(space, data, unitsToProcess, sourceBuffer->GetUnitSize(), sinkBuffer->GetUnitSize());
            sinkBuffer->WriteAcknowledge(unitsToProcess);
            sourceBuffer->ReadAcknowledge(unitsToProcess);
            unitsProcessed = unitsToProcess;
            return unitsProcessed > 0 ? true : false;
        }
        return false;
    }

    
    bool CopyFilter::Process( byte* dest, byte* source, size_t sourceUnits, size_t sourceUnitSize, size_t /*destUnitSize*/)
    {
        memcpy(dest, source, sourceUnits * sourceUnitSize);
        return true;
    }

}}//qor::pipeline