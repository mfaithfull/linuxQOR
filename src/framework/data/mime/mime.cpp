// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <cstring>
#include "mime.h"

namespace qor{ namespace mime{

    //On MSVC a shared QOR module has to export at least one symbol or no import lib
    //(.lib) is generated and a test exe that links qor_mime fails with LNK1104.
    //This is the future public API entry point of the module.
    qor_pp_export void init_mime(void)
    {
        //placeholder for future qor_mime initialisation
    }

    MIMERecognizerFilter::MIMERecognizerFilter() = default;

    MIMERecognizerFilter::~MIMERecognizerFilter() = default;

    MIMERecognizerFilter& MIMERecognizerFilter::operator = (const MIMERecognizerFilter& src) = default;

    bool MIMERecognizerFilter::ReadFilter(size_t& unitsProcessed, size_t unitsToProcess)
    {
        //Inspect the available octets first, while the source still holds them, then
        //move them on.
        size_t unitsInspected = 0;
        if(!Inspect(unitsInspected, unitsToProcess))
        {
            return false;
        }
        return TransferOctets(unitsProcessed, unitsToProcess);
    }

    bool MIMERecognizerFilter::WriteFilter(size_t& unitsProcessed, size_t unitsToProcess)
    {
        //Inspect the available octets first, while the source still holds them, then
        //move them on.
        size_t unitsInspected = 0;
        if(!Inspect(unitsInspected, unitsToProcess))
        {
            return false;
        }
        return TransferOctets(unitsProcessed, unitsToProcess);
    }

    bool MIMERecognizerFilter::TransferOctets(size_t& unitsTransferred, size_t unitsToTransfer)
    {
        if(unitsToTransfer)
        {
            auto sourceBuffer = ActualSource()->GetBuffer();
            auto sinkBuffer = ActualSink()->GetBuffer();
            if(sourceBuffer && sinkBuffer)
            {
                qor::byte* space = sinkBuffer->WriteRequest(unitsToTransfer);
                qor::byte* data = sourceBuffer->ReadRequest(unitsToTransfer);
                memcpy(space, data, unitsToTransfer * sourceBuffer->GetUnitSize());
                sinkBuffer->WriteAcknowledge(unitsToTransfer);
                sourceBuffer->ReadAcknowledge(unitsToTransfer);
                unitsTransferred = unitsToTransfer;
                return unitsTransferred > 0 ? true : false;
            }
        }
        return false;
    }

    //Plan step 2 - capture the leading octets into the signature window without consuming
    //the source. Pipeline buffer reads are a three step protocol (reserve with ReadRequest,
    //use, then commit with ReadAcknowledge or give back with ReadReject - see
    //src/framework/data/pipeline/buffer.h). Inspect requests a read of the octets it needs,
    //copies them into the window, then rejects the read so the octets are still available
    //for TransferOctets to move on to the sink untouched.
    bool MIMERecognizerFilter::Inspect(size_t& unitsInspected, size_t unitsToInspect)
    {
        unitsInspected = unitsToInspect;
        if(unitsToInspect == 0)
        {
            return true;
        }
        //The window only needs the leading octets; once it is full inspection is a no-op.
        size_t windowSpace = SIGNATURE_WINDOW_BYTES - m_signatureCount;
        if(windowSpace == 0)
        {
            return true;
        }
        auto sourceBuffer = ActualSource()->GetBuffer();
        if(sourceBuffer)
        {
            //Only capture what is available, what the window has room for and what was
            //asked for, whichever is least.
            size_t unitsToCapture = unitsToInspect;
            size_t unitsAvailable = sourceBuffer->ReadCapacity();
            if(unitsAvailable < unitsToCapture)
            {
                unitsToCapture = unitsAvailable;
            }
            if(windowSpace < unitsToCapture)
            {
                unitsToCapture = windowSpace;
            }
            if(unitsToCapture > 0)
            {
                //The request may be cut down further at the buffer wrap; use whatever it
                //gave us and reject the same amount to give the reservation back.
                size_t unitsRequested = unitsToCapture;
                qor::byte* data = sourceBuffer->ReadRequest(unitsRequested);
                if(data)
                {
                    memcpy(m_signature + m_signatureCount, data, unitsRequested * sourceBuffer->GetUnitSize());
                    m_signatureCount += unitsRequested;
                    sourceBuffer->ReadReject(unitsRequested);
                    return true;
                }
                return false;
            }
        }
        return true;
    }

    MIMEType MIMERecognizerFilter::GetDetectedType() const
    {
        return m_detectedType;
    }

    const char* MIMERecognizerFilter::GetTypeName() const
    {
        //name table for the recognised types arrives with the type detection increments
        return "";
    }

    size_t MIMERecognizerFilter::GetSignatureByteCount() const
    {
        return m_signatureCount;
    }

    void MIMERecognizerFilter::Reset(void)
    {
        m_signatureCount = 0;
        m_detectedType = MIMEType::Unresolved;
    }

}}//qor::mime
