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
    //
    //Plan step 9 support - the text markup heuristics compare text rather than fixed
    //binary magics, and HTML markup spellings are case-insensitive, so the signature
    //compares need ASCII case folding. Both octets are folded to upper case ASCII letters
    //before the compare; no other octet is folded, so the compare needs no locale and
    //pulls in nothing outside this module. It is file-local because it is an internal
    //compare helper, not part of the module interface.
    static bool markupMatch(const qor::byte* window, size_t windowCount, const qor::byte* signature, size_t signatureCount)
    {
        if(windowCount < signatureCount)
        {
            return false;
        }
        for(size_t index = 0; index < signatureCount; ++index)
        {
            qor::byte captured = window[index];
            qor::byte expected = signature[index];
            if(captured >= 0x61 && captured <= 0x7A)//lower case ASCII letter
            {
                captured = static_cast<qor::byte>(captured - 0x20);
            }
            if(expected >= 0x61 && expected <= 0x7A)//lower case ASCII letter
            {
                expected = static_cast<qor::byte>(expected - 0x20);
            }
            if(captured != expected)
            {
                return false;
            }
        }
        return true;
    }

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
                    //Plan steps 3,4,5,6,7,8,9,10,11,12 - classify the captured leading octets. The evaluation
                    //order is fixed so results are deterministic (most definite signature
                    //first): png, jpeg, gif, pdf, zip, then the text markups, then the
                    //fallback. Each guard asks only for the octets its own signature needs,
                    //so a shorter signature can decide before the window is full. The
                    //signature window is never consumed (the read was rejected), so these
                    //checks run again on every chunk as more leading octets arrive. The
                    //octet-stream fallback is added one test at a time after this.
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
                    //Plan step 8 - an XML document normally begins with the optional XML
                    //declaration whose fixed opening marker is the five characters <?xml
                    //('<' '?' 'x' 'm' 'l'). Unlike the binary formats above, XML has no
                    //magic number, so this is a strong text heuristic evaluated after the
                    //definite binary signatures in the fixed order. The guard asks for five
                    //octets only, so it can decide before the window is full.
                    if(m_detectedType == MIMEType::Unresolved && m_signatureCount >= 5)
                    {
                        static const qor::byte xmlSignature[5] =
                            { 0x3C, 0x3F, 0x78, 0x6D, 0x6C };//'<' '?' 'x' 'm' 'l'
                        if(memcmp(m_signature, xmlSignature, sizeof(xmlSignature)) == 0)
                        {
                            m_detectedType = MIMEType::XML;
                        }
                    }
                    //Plan step 9 - an HTML document normally begins with the doctype
                    //declaration "<!DOCTYPE html ..." (the HTML5 form, fourteen octets up to
                    //and including the word HTML) or, when no doctype is present, with a
                    //document level tag: the first '<' starts a tag named html, head, body or
                    //title. Like XML, HTML has no binary magic number, so both checks are text
                    //heuristics evaluated after the XML declaration marker in the fixed order.
                    //Unlike the magics above, the compares are case-insensitive because HTML
                    //markup spellings are case-insensitive, so they use the ASCII case-fold
                    //helper markupMatch rather than memcmp. markupMatch also checks the window
                    //holds enough octets, so each check can decide before the window is full.
                    if(m_detectedType == MIMEType::Unresolved)
                    {
                        static const qor::byte doctypeHtmlSignature[14] =
                            { 0x3C, 0x21, 0x44, 0x4F, 0x43, 0x54, 0x59, 0x50, 0x45, 0x20, 0x48, 0x54, 0x4D, 0x4C };
                            //'<' '!' 'D' 'O' 'C' 'T' 'Y' 'P' 'E' ' ' 'H' 'T' 'M' 'L'
                        if(markupMatch(m_signature, m_signatureCount, doctypeHtmlSignature, 14))
                        {
                            m_detectedType = MIMEType::HTML;
                        }
                        //The document level tag check: the very first octet is '<' and the tag
                        //name that follows it, case-folded, is one of the four document level
                        //tags. The shortest name is four octets, so five window octets are
                        //enough to decide; markupMatch rejects the five octet title name when
                        //the window is too short.
                        if(m_detectedType == MIMEType::Unresolved && m_signatureCount >= 5 &&
                            m_signature[0] == 0x3C)//'<'
                        {
                            static const qor::byte htmlTag[4] = { 0x68, 0x74, 0x6D, 0x6C };//'h' 't' 'm' 'l'
                            static const qor::byte headTag[4] = { 0x68, 0x65, 0x61, 0x64 };//'h' 'e' 'a' 'd'
                            static const qor::byte bodyTag[4] = { 0x62, 0x6F, 0x64, 0x79 };//'b' 'o' 'd' 'y'
                            static const qor::byte titleTag[5] = { 0x74, 0x69, 0x74, 0x6C, 0x65 };//'t' 'i' 't' 'l' 'e'
                            if(markupMatch(m_signature + 1, m_signatureCount - 1, htmlTag, 4)
                                || markupMatch(m_signature + 1, m_signatureCount - 1, headTag, 4)
                                || markupMatch(m_signature + 1, m_signatureCount - 1, bodyTag, 4)
                                || markupMatch(m_signature + 1, m_signatureCount - 1, titleTag, 5))
                            {
                                m_detectedType = MIMEType::HTML;
                            }
                        }
                    }
                    //Plan step 10 - a JSON document begins with a value, and the only two values
                    //that can be recognised from a signature window are the two containers: the
                    //object brace and the array bracket. JSON has no magic number, so this is a
                    //text heuristic evaluated after the HTML markup checks in the fixed order. It
                    //is the first octet that is not JSON white space that decides, and the only
                    //four characters JSON allows as whitespace are space, tab, line feed and
                    //carriage return. Unlike the markup compares above, this compare is not case
                    //relevant, so a plain octet compare is enough and markupMatch is not needed.
                    //It can decide as soon as a single non-whitespace octet has been captured, so
                    //it does not need the window to be full.
                    if(m_detectedType == MIMEType::Unresolved)
                    {
                        for(size_t index = 0; index < m_signatureCount; ++index)
                        {
                            const qor::byte octet = m_signature[index];
                            if(octet == 0x20 || octet == 0x09 || octet == 0x0A || octet == 0x0D)//JSON whitespace: ' ' tab line feed carriage return
                            {
                                continue;
                            }
                            if(octet == 0x7B || octet == 0x5B)//'{' or '['
                            {
                                m_detectedType = MIMEType::JSON;
                            }
                            break;
                        }
                    }
                    //Plan step 11 - JavaScript has no magic number, so this is a weak text
                    //heuristic evaluated after the JSON container check in the fixed order
                    //(png, jpeg, gif, pdf, zip, xml, html, json, javascript, ...). Three leading
                    //signatures are recognised in this increment, any of which is a strong enough
                    //indicator of a JavaScript source to classify the stream:
                    //  (1) a shebang - the first two octets are the hash-bang "#!" (0x23 0x21), the
                    //      interpreter line that leads a standalone script file;
                    //  (2) a top-level function declaration - the window opens with the ECMA keyword
                    //      "function" followed by a space. JavaScript is case-sensitive, so unlike
                    //      the HTML markup compares above this uses a plain byte compare (memcmp),
                    //      not the case-folded markupMatch;
                    //  (3) a leading source comment - the window opens with a line comment "//" (0x2F
                    //      0x2F) or a block comment "/*" (0x2F 0x2A). A stream that begins with a
                    //      source comment is a source file, and within this set of recognised types
                    //      that means JavaScript.
                    //The heuristic is deliberately narrow so it cannot fire on the markup or container
                    //streams recognised above. The arrow form "=>" is contemplated as a further weak
                    //signature but is not yet exercised by a test, so it is not implemented here.
                    if(m_detectedType == MIMEType::Unresolved && m_signatureCount >= 2 &&
                        m_signature[0] == 0x23 && m_signature[1] == 0x21)//shebang "#!"
                    {
                        m_detectedType = MIMEType::JavaScript;
                    }
                    if(m_detectedType == MIMEType::Unresolved && m_signatureCount >= 9)
                    {
                        static const qor::byte functionKeyword[9] = { 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x20 };//'f' 'u' 'n' 'c' 't' 'i' 'o' 'n' ' '
                        if(memcmp(m_signature, functionKeyword, 9) == 0)
                        {
                            m_detectedType = MIMEType::JavaScript;
                        }
                    }
                    if(m_detectedType == MIMEType::Unresolved && m_signatureCount >= 2 &&
                        m_signature[0] == 0x2F && (m_signature[1] == 0x2F || m_signature[1] == 0x2A))//comment "//" or "/*"
                    {
                        m_detectedType = MIMEType::JavaScript;
                    }

                    //Plan step 12 - plain text is the default for text: it has no magic number, no
                    //markup marker, no container opener and no source file leading signature, so it is
                    //recognised only after every definite signature above has failed to match. Two
                    //things make this the weakest of the guards:
                    //  (1) the whole signature window must have been captured, so a binary stream
                    //      cannot look like text before the control octets that betray it have arrived
                    //      - a partially captured window is never enough to call a stream text;
                    //  (2) every octet in the window must be printable: printable ASCII (0x20-0x7E,
                    //      the space included), the three controls the text and JSON conventions allow
                    //      (tab, line feed and carriage return) and the high octets (0x80-0xFF) that
                    //      carry UTF-8 encoded text. Well formed UTF-8 is deliberately not checked
                    //      here - a window of printable octets is text for the purposes of this
                    //      recognizer, and full UTF-8 validation belongs to the Unicode layer, which
                    //      this framework layer module must not depend on.
                    //Any other control octet (a NUL, the other C0 controls or DEL) leaves the stream
                    //unresolved so that the octet-stream fallback (plan step 13) can classify it.
                    if(m_detectedType == MIMEType::Unresolved && m_signatureCount >= SIGNATURE_WINDOW_BYTES)
                    {
                        bool allPrintable = true;
                        for(size_t index = 0; index < m_signatureCount; ++index)
                        {
                            const qor::byte octet = m_signature[index];
                            if(octet >= 0x80)//UTF-8 encoded text
                            {
                                continue;
                            }
                            if(octet >= 0x20 && octet <= 0x7E)//printable ASCII
                            {
                                continue;
                            }
                            if(octet == 0x09 || octet == 0x0A || octet == 0x0D)//tab, line feed, carriage return
                            {
                                continue;
                            }
                            allPrintable = false;
                            break;
                        }
                        if(allPrintable)
                        {
                            m_detectedType = MIMEType::PlainText;
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
