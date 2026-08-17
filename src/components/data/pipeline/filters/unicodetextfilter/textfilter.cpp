// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <cmath>
#include "textfilter.h"

namespace qor{ namespace text { namespace components {

    UnicodeTextFilter::UnicodeTextFilter() : pipeline::InlineProcessor<uint32_t, byte>(128, 128), 
        m_inputFormat(TextInputFormat::ascii), m_nextHandler(qor_pp_make_function(&UnicodeTextFilter::FirstByteHandler))
    { }

    UnicodeTextFilter::~UnicodeTextFilter() = default;

    void UnicodeTextFilter::SetInputFormat(TextInputFormat format)
    {
        m_inputFormat = format;
    }

    void UnicodeTextFilter::Process(size_t& itemCount)
    {
        if(itemCount > 0)
        {
            size_t readTarget = itemCount;
            size_t totalCount = 0;
            do
            {
                ProcessInput(itemCount, readTarget, totalCount);
            }while(readTarget > 0 && m_sourceBuffer.ReadCapacity() > 0 && m_sinkBuffer.WriteCapacity() > 0);
            itemCount = totalCount;
        }
    }

    void UnicodeTextFilter::ProcessInput(size_t& itemCount, size_t& readTarget, size_t& totalCount)
    {
        SetupState();
        m_data = m_sourceBuffer.ReadRequest(itemCount);
        if(m_data)
        {
            m_inputSize = itemCount;
            m_writeSize = EstimateOutputCount(m_inputSize);
            m_space = reinterpret_cast<uint32_t*>(m_sinkBuffer.WriteRequest(m_writeSize));
            if(m_space)
            {
                DecodeToUTF32();
            }
        }
        readTarget -= m_inputIndex;
        totalCount += m_inputIndex;
        itemCount = readTarget;
    }

    void UnicodeTextFilter::DecodeToUTF32()
    {
        while(m_nextHandler(*this)){ };        
        m_writeCount > 0 ? m_sinkBuffer.WriteAcknowledge(m_writeCount) : m_sinkBuffer.WriteReject();
        m_inputIndex > 0 ? m_sourceBuffer.ReadAcknowledge(m_inputIndex) : m_sourceBuffer.ReadReject(m_inputSize);
    }

    void UnicodeTextFilter::SetupState()
    {
        byte* m_data = nullptr;
        uint32_t* m_space = nullptr;
        m_inputIndex = 0;
        m_inputSize = 0;
        m_writeCount = 0;
        m_writeSize = 0;
    }

    void UnicodeTextFilter::Reset(size_t inItemCount, size_t outItemCount)
    {
        m_inputFormat = TextInputFormat::ascii;
        m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::FirstByteHandler);
        m_utf8Index = 0;
        m_decodedLength = 0;
        memset(m_utf8Bytes, 0, 6);
        m_utf32 = 0;
        m_skipNextWhiteSpace = 0;

        SetupState();
        pipeline::InlineProcessor<uint32_t, byte>::Reset(inItemCount, outItemCount);
    }

    size_t UnicodeTextFilter::EstimateOutputCount(size_t inputCount)
    {
        if(inputCount == 0)
        {
            return 0;
        }
        size_t outputEstimate = inputCount;
        switch(m_inputFormat)
        {
            case TextInputFormat::ascii:
                break;
            case TextInputFormat::utf8:
                outputEstimate = static_cast<size_t>(std::floor( static_cast<double>(inputCount) / 1.5));
                break;
            case TextInputFormat::beutf16:
            case TextInputFormat::leutf16:
                outputEstimate = inputCount / 2;
                break;
            case TextInputFormat::beutf32:
            case TextInputFormat::leutf32:
                outputEstimate = inputCount / 4;
                break;
        }
        return outputEstimate + 1;
    }

    bool UnicodeTextFilter::FirstByteHandler()
    {        
        if(m_inputIndex >= m_inputSize || m_writeCount >= m_writeSize)
        {
            return false;
        }

        switch (m_data[m_inputIndex])
        {
            case 0x00:
                //Matches first byte of UTF-32BE only
                m_inputIndex++;                
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32BESecondBOMByte);
            break;
            case  0xEF:
                //Matches first byte of UTF-8 only
                m_inputIndex++;
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF8SecondBOMByte);
            break;
            case 0xFE:
                //Matches first byte of UTF-16BE only
                m_inputIndex++;
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF16BESecondBOMByte);
            break;
            case 0XFF:
                //Match LE BOMs
                m_inputIndex++;
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::LEBOMByte2Handler);
            break;
            default:
                m_inputFormat = TextInputFormat::ascii;
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::ASCIIHandler);
                break;
        }
        return true;
    }

    bool UnicodeTextFilter::LEBOMByte2Handler()
    {        
        if(m_inputIndex >= m_inputSize)
        {
            return false;
        }

        switch (m_data[m_inputIndex])
        {
            case 0xFE:                
                m_inputIndex++;                
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::LEBOMByte3Handler);
            break;
            default:
                m_inputIndex = 0;
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::ErrorHandler);
                break;
        }
        return true;
    }

    bool UnicodeTextFilter::LEBOMByte3Handler()
    {
        if(m_inputIndex >= m_inputSize)
        {
            return false;
        }
        switch (m_data[m_inputIndex])
        {
            case 0x00:                
                m_inputIndex++;                
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::LEUTF32BOMByte4Handler);
            break;
            default:
                m_inputFormat = TextInputFormat::leutf16;                
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF16LEByte1Handler);
                break;
        }
        return true;
    }

    bool UnicodeTextFilter::LEUTF32BOMByte4Handler()
    {
        if(m_inputIndex >= m_inputSize)
        {
            return false;
        }
        switch (m_data[m_inputIndex])
        {
            case 0x00:
                m_inputFormat = TextInputFormat::leutf32;
                m_inputIndex++;
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32LEByte1Handler);
            break;
            default:
                m_inputIndex = 0;
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::ErrorHandler);
            break;
        }
        return true;
    }
    
    bool UnicodeTextFilter::UTF8SecondBOMByte()
    {
        if(m_inputIndex >= m_inputSize || m_writeCount >= m_writeSize)
        {
            return false;
        }

        if(m_data[m_inputIndex] == 0xBB)
        {
            m_inputIndex++;
            m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF8ThirdBOMByte);
        }
        else
        {
            if(m_inputIndex > 0)
            {
                m_inputIndex--;
            }
            m_inputFormat = TextInputFormat::ascii;
            m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::ASCIIHandler);
        }
        return true;
    }

    bool UnicodeTextFilter::UTF16BESecondBOMByte()
    {
        if(m_inputIndex >= m_inputSize || m_writeCount >= m_writeSize)
        {
            return false;
        }
        if(m_data[m_inputIndex] == 0xFF)
        {
            m_inputFormat = TextInputFormat::beutf16;
            m_inputIndex++;
            m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF16BEByte1Handler);
        }
        else
        {
            m_inputIndex = 0;
            m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::ErrorHandler);
        }
        return true;
    }

    bool UnicodeTextFilter::UTF32BESecondBOMByte()
    {
        if(m_inputIndex >= m_inputSize || m_writeCount >= m_writeSize)
        {
            return false;
        }
        if(m_data[m_inputIndex] == 0x00)
        {
            m_inputIndex++;
            m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32BEThirdBOMByte);
        }
        else
        {
            m_inputIndex = 0;
            m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::ErrorHandler);
        }
        return true;
    }

    bool UnicodeTextFilter::UTF8ThirdBOMByte()
    {
        if(m_inputIndex >= m_inputSize || m_writeCount >= m_writeSize)
        {
            return false;
        }

        if(m_data[m_inputIndex] == 0xBF)
        {
            m_inputFormat = TextInputFormat::utf8;
            m_inputIndex++;
            m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF8LeadByteHandler);
        }
        else
        {
            m_inputIndex = 0;
            m_inputFormat = TextInputFormat::ascii;
            m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::ASCIIHandler);
        }
        return true;
    }

    bool UnicodeTextFilter::UTF32BEThirdBOMByte()
    {
        if(m_inputIndex >= m_inputSize || m_writeCount >= m_writeSize)
        {
            return false;
        }
        if(m_data[m_inputIndex] == 0xFE)
        {
            m_inputIndex++;
            m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32BEFourthBOMByte);
        }
        else
        {
            m_inputIndex = 0;
            m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::ErrorHandler);
        }
        return true;
    }

    bool UnicodeTextFilter::UTF32BEFourthBOMByte()
    {
        if(m_inputIndex >= m_inputSize || m_writeCount >= m_writeSize)
        {
            return false;
        }
        if(m_data[m_inputIndex] == 0xFF)
        {
            m_inputFormat = TextInputFormat::beutf32;
            m_inputIndex++;
            m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32BEByte1Handler);
        }
        else
        {
            m_inputIndex = 0;
            m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::ErrorHandler);
        }
        return true;
    }

    bool UnicodeTextFilter::UTF8LeadByteHandler()
    {
        if(m_inputIndex >= m_inputSize)
        {
            return false;
        }

        m_decodedLength = codepoint_decoded_length[static_cast<char8_t>(m_data[m_inputIndex])];
        m_utf8Index = 0;
        //memset(m_utf8Bytes, 0, 6);

        switch(m_decodedLength)
        {
            case 0:
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::ErrorHandler);
                break;
            case 1:
                if(m_writeCount >= m_writeSize)
                {
                    return false;
                }
                m_space[m_writeCount++] = static_cast<uint32_t>(m_data[m_inputIndex++]);
                return true;
                break;
            case 2:
            case 3:
            case 4:
                m_utf8Bytes[m_utf8Index++] = m_data[m_inputIndex++];
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF8FollowByteHandler);
                return true;
                break;
            case 5:
            case 6:
            default:
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::ErrorHandler);
                return true;
                break;

        }
        return false;
    }

    bool UnicodeTextFilter::UTF8FollowByteHandler()
    {
        if(m_utf8Index < m_decodedLength)
        {
            if(m_inputIndex >= m_inputSize)
            {
                return false;
            }
            m_utf8Bytes[m_utf8Index++] = m_data[m_inputIndex++];
        }

        if(m_utf8Index == m_decodedLength)
        {
            if(m_writeCount >= m_writeSize)
            {
                return false;
            }

            uint32_t codePoint;

            switch(m_decodedLength)
            {
                case 1:
                    codePoint = static_cast<uint32_t>(m_utf8Bytes[0]);
                    break;
                case 2:
                    codePoint = (m_utf8Bytes[0] & 0x1F) << 6 | (m_utf8Bytes[1] & 0x3F);
                    break;
                case 3:
                    codePoint = (m_utf8Bytes[0] & 0x0F) << 12 | (m_utf8Bytes[1] & 0x3F) << 6 | (m_utf8Bytes[2] & 0x3F);
                    break;
                case 4:
                    codePoint = (m_utf8Bytes[0] & 0x07) << 18 | (m_utf8Bytes[1] & 0x3F) << 12 | (m_utf8Bytes[2] & 0x3F) << 6 | (m_utf8Bytes[3] & 0x3F);
                    break;
            }

            m_space[m_writeCount++] = codePoint;
            m_utf8Index = 0;
            m_decodedLength = 0;
            m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF8LeadByteHandler);
        }
        return true;
    }

    bool UnicodeTextFilter::ErrorHandler()
    {
        if(m_writeCount >= m_writeSize)
        {
            return false;
        }

        m_space[m_writeCount++] = m_replacementCodePoint;
        switch(m_inputFormat)
        {
            case TextInputFormat::ascii:                
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::ASCIIHandler);
                break;
            case TextInputFormat::utf8:
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF8LeadByteHandler);
                break;
            case TextInputFormat::beutf16:                
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF16BEByte1Handler);
                break;
            case TextInputFormat::beutf32:
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32BEByte1Handler);
                break;
            case TextInputFormat::leutf16:                
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF16LEByte1Handler);
                break;
            case TextInputFormat::leutf32:
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32LEByte1Handler);                
                break;
        }
        return true;        
    }

    bool UnicodeTextFilter::ASCIIHandler()
    {        
        while (m_inputIndex < m_inputSize && m_writeCount < m_writeSize)
        {
            m_space[m_writeCount++] = static_cast<uint32_t>(m_data[m_inputIndex++]);
        }
        return false;
    }

    bool UnicodeTextFilter::ASCIIHandlerSkipConsectiveWhitespace()
    {        
        while (m_inputIndex < m_inputSize && m_writeCount < m_writeSize)
        {
            uint32_t ASCIICharacter = static_cast<uint32_t>(m_data[m_inputIndex++]);
            bool whiteSpace = (ASCIICharacter == 0x000A || ASCIICharacter == 0x000D || ASCIICharacter == 0x0009 || ASCIICharacter == 0x0020);
            if(!(m_skipNextWhiteSpace && whiteSpace))
            {
                m_space[m_writeCount++] = ASCIICharacter;
            }
            m_skipNextWhiteSpace = whiteSpace;            
        }
        return false;
    }

    bool UnicodeTextFilter::UTF32BEByte1Handler()
    {
        if(m_inputIndex >= m_inputSize)
        {
            return false;
        }        
        m_utf32 = static_cast<uint32_t>(m_data[m_inputIndex++]);
        m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32BEByte2Handler);
        return true;
    }

    bool UnicodeTextFilter::UTF32BEByte2Handler()
    {
        if(m_inputIndex >= m_inputSize)
        {
            return false;
        }        
        m_utf32 = ( m_utf32 << 8 ) | m_data[m_inputIndex++];
        m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32BEByte3Handler);
        return true;
    }

    bool UnicodeTextFilter::UTF32BEByte3Handler()
    {
        if(m_inputIndex >= m_inputSize)
        {
            return false;
        }        
        m_utf32 = ( m_utf32 << 8 ) | m_data[m_inputIndex++];
        m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32BEByte4Handler);
        return true;
    }

    bool UnicodeTextFilter::UTF32BEByte4Handler()
    {
        if(m_inputIndex >= m_inputSize || m_writeCount >= m_writeSize)
        {
            return false;
        }

        m_utf32 = ( m_utf32 << 8 ) | m_data[m_inputIndex++];
        //TODO: flip m_utf32be to native if we need to.
        m_space[m_writeCount++] = m_utf32;
        m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32BEByte1Handler);
        return true;
    }

    bool UnicodeTextFilter::UTF16BEByte1Handler()
    {
        if(m_inputIndex >= m_inputSize || m_writeCount >= m_writeSize)
        {
            return false;
        }

        m_utf32 = static_cast<uint32_t>(m_data[m_inputIndex++]);
        m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF16BEByte2Handler);
        return true;
    }
    
    bool UnicodeTextFilter::UTF16BEByte2Handler()
    {
        if(m_inputIndex >= m_inputSize || m_writeCount >= m_writeSize)
        {
            return false;
        }

        m_utf32 = ( m_utf32 << 8 ) | m_data[m_inputIndex++];
        //TODO: flip m_utf32 to native if we need to.
        m_space[m_writeCount++] = m_utf32;
        m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF16BEByte1Handler);
        return true;
    }

    bool UnicodeTextFilter::UTF32LEByte1Handler()
    {
        if(m_inputIndex >= m_inputSize || m_writeCount >= m_writeSize)
        {
            return false;
        }

        m_utf32 = static_cast<uint32_t>(m_data[m_inputIndex++]);
        m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32LEByte2Handler);
        return true;
    }

    bool UnicodeTextFilter::UTF32LEByte2Handler()
    {
        if(m_inputIndex >= m_inputSize)
        {
            return false;
        }        
        m_utf32 = ( m_utf32 << 8 ) | m_data[m_inputIndex++];
        m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32LEByte3Handler);
        return true;
    }

    bool UnicodeTextFilter::UTF32LEByte3Handler()
    {
        if(m_inputIndex >= m_inputSize)
        {
            return false;
        }        
        m_utf32 = ( m_utf32 << 8 ) | m_data[m_inputIndex++];
        m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32LEByte4Handler);
        return true;
    }

    bool UnicodeTextFilter::UTF32LEByte4Handler()
    {
        if(m_inputIndex >= m_inputSize || m_writeCount >= m_writeSize)
        {
            return false;
        }

        m_utf32 = ( m_utf32 << 8 ) | m_data[m_inputIndex++];
        //TODO: flip m_utf32be to native if we need to.
        m_space[m_writeCount++] = m_utf32;
        m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF32LEByte1Handler);
        return true;
    }

    bool UnicodeTextFilter::UTF16LEByte1Handler()
    {
        while(!(m_inputIndex >= m_inputSize || m_writeCount >= m_writeSize))
        {
            m_utf8Bytes[ qor::arch::endian == qor::arch::Endian::little ? 0 : 1 ] = m_data[m_inputIndex++];
            if(!UTF16LEByte2Handler())
            {
                m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF16LEByte2Handler);
                break;
            }
        }
        return false;
    }

    bool UnicodeTextFilter::UTF16LEByte2Handler()
    {
        if(m_inputIndex >= m_inputSize || m_writeCount >= m_writeSize)
        {
            return false;
        }

        m_utf8Bytes[ qor::arch::endian == qor::arch::Endian::little ? 1 : 0 ] = m_data[m_inputIndex++];

        char16_t c = *(reinterpret_cast<char16_t*>(&m_utf8Bytes[0]));
        if (c >= 0xD800 && c <= 0xDFFF)
        {
            char32_t highDecode = ((c - 0xD800) << 10);
            //unpaired surrogate. Windows sometimes does this so allow it
            m_space[m_writeCount++];
        }
        else
        {
            m_space[m_writeCount++] = static_cast<uint32_t>(c);				
        }
        m_nextHandler = qor_pp_make_function(&UnicodeTextFilter::UTF16LEByte1Handler);
        return true;
    }

}}}//qor::text::components