// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_STATES_ONEOFARANGE
#define QOR_PP_H_DATA_PARSER_STATES_ONEOFARANGE

#include "../state.h"

namespace qor { namespace data { namespace parser {

    //A single byte range match
    class qor_pp_module_interface(QOR_PARSER) OneOfAByteRange : public ParserState
    {
    public:

        OneOfAByteRange(Parser* parser, byte firstOctet, byte lastOctet, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~OneOfAByteRange();
    private:
        ByteContext* m_context;
        byte m_first;
        byte m_last;
    };

    template<typename range_t>
    class OneOfARange_t : public ParserState
    {
    public:

        OneOfARange_t(Parser* parser, range_t first, range_t last, uint64_t token = static_cast<uint64_t>(eToken::Lexical)) : ParserState(parser, token),
            m_first(first), m_last(last)
        {            
            Enter = [this]()
                {
                    Prepare();
                    uint32_t* data = nullptr;
                    m_context = static_cast< pipeline::BufferContext<range_t>* >(GetParser()->GetContext());
                    if (m_context->GetItem(data))
                    {
                        m_result.m_position = m_context->GetPosition();
                        if ((*data >= m_first && *data <= m_last))
                        {                        
                            m_result.first = *data;                        
                            m_context->ConsumeItem();
                            m_result.token = m_token;
                            ++m_result.length;
                            m_result.code = Result::SUCCESS;
                        }
                        else
                        {        
                            m_context->RejectItem();
                            m_result.code = Result::FAILURE;
                            m_result.length = 0;
                        }
                        Workflow()->PopStep();
                    }
                    else
                    {
                        //log::debug("Ran out of data looking for a range '{0:x02}'-'{1:x02}'", (char)m_first, (char)m_last);
                        m_result.code = Result::MORE_DATA;//Need more data
                        if(GetParser()->IsFinal())
                        {
                            m_result.code = Result::FAILURE;
                            Workflow()->PopStep();
                        }
                    }
                };
        }


        virtual ~OneOfARange_t() = default;

    private:
        pipeline::BufferContext<range_t>* m_context;
        range_t m_first;
        range_t m_last;

    };

}}}

namespace qor{
    qor_pp_declare_source_of(data::parser::OneOfAByteRange, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::OneOfAByteRange>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_PARSER_STATES_ONEOFARANGE
