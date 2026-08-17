// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_FILTER_UNICODE_TEXT
#define QOR_PP_H_PIPELINE_FILTER_UNICODE_TEXT

#include "src/framework/data/pipeline/inlineprocessor.h"
#include "src/qor/function/tef.h"
#include "src/qor/function/make_function.h"

namespace qor{ namespace text{ namespace components {

    enum class TextInputFormat
    {
        ascii,
        utf8,
        beutf16,
        beutf32,
        leutf16,
        leutf32,
    };

    class qor_pp_module_interface_gcc(QOR_UTEXTFILTER) UnicodeTextFilter : public pipeline::InlineProcessor<uint32_t, byte>
    {
    public:

        qor_pp_module_interface(QOR_UTEXTFILTER) UnicodeTextFilter();        
        qor_pp_module_interface(QOR_UTEXTFILTER) virtual ~UnicodeTextFilter();
        qor_pp_module_interface(QOR_UTEXTFILTER) virtual void Process(size_t& itemCount) override;
        qor_pp_module_interface(QOR_UTEXTFILTER) void SetInputFormat(TextInputFormat format);
        qor_pp_module_interface(QOR_UTEXTFILTER) virtual void Reset(size_t inItemCount, size_t outItemCount = 1);

    private:

        TextInputFormat m_inputFormat;
        inline size_t EstimateOutputCount(size_t inputCount);
        inline void SetupState();
        inline void ProcessInput(size_t& itemCount, size_t& readTarget, size_t& totalCount);
        inline void DecodeToUTF32();

        bool FirstByteHandler();
        bool LEBOMByte2Handler();
        bool LEBOMByte3Handler();
        bool LEUTF32BOMByte4Handler();
        bool ASCIIHandler();
        bool ASCIIHandlerSkipConsectiveWhitespace();
        bool UTF8SecondBOMByte();
        bool UTF8ThirdBOMByte();
        bool UTF8LeadByteHandler();
        bool UTF8FollowByteHandler();
        bool ErrorHandler();
        bool UTF16BESecondBOMByte();
        bool UTF32BESecondBOMByte();
        bool UTF32BEThirdBOMByte();
        bool UTF32BEFourthBOMByte();
        bool UTF32BEByte1Handler();
        bool UTF32BEByte2Handler();
        bool UTF32BEByte3Handler();
        bool UTF32BEByte4Handler();
        bool UTF16BEByte1Handler();
        bool UTF16BEByte2Handler();
        bool UTF32LEByte1Handler();
        bool UTF32LEByte2Handler();
        bool UTF32LEByte3Handler();
        bool UTF32LEByte4Handler();
        bool UTF16LEByte1Handler();
        bool UTF16LEByte2Handler();

  	    tef::MemberFunction<bool (UnicodeTextFilter::* )()> m_nextHandler;

        byte* m_data;
        uint32_t* m_space;
        size_t m_inputIndex;
        size_t m_inputSize;
        size_t m_writeCount;
        size_t m_writeSize;
        unsigned char m_utf8Index;
        unsigned char m_decodedLength;
        byte m_utf8Bytes[6];
        uint32_t m_utf32;
        bool m_skipNextWhiteSpace{false};
        
        const uint32_t m_replacementCodePoint{0xFFFD};
    };

}}}//qor::text::components

#endif//QOR_PP_H_PIPELINE_FILTER_UNICODE_TEXT
