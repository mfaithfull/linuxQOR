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
                    //Plan steps 3,4,5,6,7 - classify the captured leading octets. The evaluation
                    //order is fixed so results are deterministic (most definite signature
                    //first): png, jpeg, gif, pdf, zip, then the text markups, then the
                    //fallback. Each guard asks only for the octets its own signature needs,
                    //so a shorter signature can decide before the window is full. The
                    //signature window is never consumed (the read was rejected), so these
                    //checks run again on every chunk as more leading octets arrive. The
                    //remaining five types are added one test at a time after this.
                    if(m_detectedType == MIMEType::Unresolved && m_signatureCount >= 8)
                    {
                        static const qor::byte pngSignature[8] =
                            { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
                        if(memcmp(m_signature, pngSignature, sizeof(pngSignature)) == 0)
                        {
                            m_detectedType = MIMEType::PNG;
                        }
                    }
                    if(m_detectedType == MIMEType::Unresolved && m_signatureCount >= 3)
                    {
                        static const qor::byte jpegSignature[3] =
                            { 0xFF, 0xD8, 0xFF };
                        if(memcmp(m_signature, jpegSignature, sizeof(jpegSignature)) == 0)
                        {
                            m_detectedType = MIMEType::JPEG;
                        }
                    }
                    //Plan step 5 - the GIF spec defines two header variants, GIF89a and GIF87a,
                    //both six bytes, so both are compared. The guard asks for six octets only.
                    if(m_detectedType == MIMEType::Unresolved && m_signatureCount >= 6)
                    {
                        static const qor::byte gif89Signature[6] =
                            { 0x47, 0x49, 0x46, 0x38, 0x39, 0x61 };//'G' 'I' 'F' '8' '9' 'a'
                        static const qor::byte gif87Signature[6] =
                            { 0x47, 0x49, 0x46, 0x38, 0x37, 0x61 };//'G' 'I' 'F' '8' '7' 'a'
                        if(
                            memcmp(m_signature, gif89Signature, sizeof(gif89Signature)) == 0
                            || memcmp(m_signature, gif87Signature, sizeof(gif87Signature)) == 0
                        )
                        {
                            m_detectedType = MIMEType::GIF;
                        }
                    }
                    //Plan step 6 - the PDF file format begins with the four byte ASCII
                    //signature %PDF (percent P D F), normally followed by the version
                    //marker. The guard asks for four octets only, so it can decide before
                    //the window is full.
                    if(m_detectedType == MIMEType::Unresolved && m_signatureCount >= 4)
                    {
                        static const qor::byte pdfSignature[4] =
                            { 0x25, 0x50, 0x44, 0x46 };//'%' 'P' 'D' 'F'
                        if(memcmp(m_signature, pdfSignature, sizeof(pdfSignature)) == 0)
                        {
                            m_detectedType = MIMEType::PDF;
                        }
                    }
                    //Plan step 7 - ZIP based archives begin with the four byte signature PK 03 04,
                    //the ASCII characters 'P' 'K' followed by the version numbers 03 04 that marks
                    //a local file header. The guard asks for four octets only, so it can decide
                    //before the window is full.
                    if(m_detectedType == MIMEType::Unresolved && m_signatureCount >= 4)
                    {
                        static const qor::byte zipSignature[4] =
                            { 0x50, 0x4B, 0x03, 0x04 };//'P' 'K' 03 04
                        if(memcmp(m_signature, zipSignature, sizeof(zipSignature)) == 0)
                        {
                            m_detectedType = MIMEType::ZIP;
                        }
                    }
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

    //The registered MIME name for each enum value (see the MIMEType enum in mime.h).
    //Unresolved has no name. The table is complete so that GetTypeName() is correct for
    //every type the recognizer can report; which types are actually reachable is decided
    //one at a time by the signature checks in Inspect().
    const char* MIMERecognizerFilter::GetTypeName() const
    {
        switch(m_detectedType)
        {
            case MIMEType::PNG:          return "image/png";
            case MIMEType::JPEG:         return "image/jpeg";
            case MIMEType::GIF:          return "image/gif";
            case MIMEType::PDF:          return "application/pdf";
            case MIMEType::ZIP:          return "application/zip";
            case MIMEType::XML:          return "application/xml";
            case MIMEType::HTML:         return "text/html";
            case MIMEType::JSON:         return "application/json";
            case MIMEType::JavaScript:   return "application/javascript";
            case MIMEType::PlainText:    return "text/plain";
            case MIMEType::OctetStream:  return "application/octet-stream";
            case MIMEType::Unresolved:
            default:                     return "";
        }
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
