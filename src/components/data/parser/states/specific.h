// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_STATES_SPECIFIC
#define QOR_PP_H_DATA_PARSER_STATES_SPECIFIC

#include "../state.h"

namespace qor { namespace data { namespace parser {

    //matches one specific item only
    template<typename item_t>
    class Specific : public ParserState
    {
    public:
        Specific(Parser* parser, item_t matchingItem, uint64_t token = static_cast<uint64_t>(eToken::Lexical)) : ParserState(parser, token),
            m_matchingItem(matchingItem)
        {
        Enter = [this]()
            {
                Prepare();
                item_t* data = nullptr;
                pipeline::BufferContext<item_t>* context = dynamic_cast<pipeline::BufferContext<item_t>*>(GetContext());
                if (context->GetItem(data))
                {
                    if (*data == m_matchingItem)
                    {
                        m_result.first = *data;
                        m_result.m_position = context->GetPosition();
                        context->ConsumeItem();
                        m_result.token = m_token;
                        m_result.length = 1;
                        m_result.code = Result::SUCCESS;
                        //log::debug("Matched '{0}'", (item_t)m_matchingItem);
                        Workflow()->PopStep();
                    }
                    else if (data)
                    {
                        context->RejectItem();
                        //log::debug("{0} != {1}", (item_t)*data, (item_t)m_matchingItem);
                        m_result.code = Result::FAILURE;
                        m_result.m_position = context->GetPosition();
                        m_result.length = 0;
                        Workflow()->PopStep();
                    }
                    else
                    {
                        m_result.code = Result::MORE_DATA;
                    }
                }
                else
                {
                    m_result.code = Result::MORE_DATA;//Need more data to continue
                    if (GetParser()->IsFinal())
                    {
                        m_result.code = Result::FAILURE;
                        Workflow()->PopStep();
                    } 
                }
            };
        }

        virtual ~Specific() = default;
        virtual void Emit(){ };
    private:
        item_t m_matchingItem;
    };

}}}//qor::data::parser

#endif//QOR_PP_H_DATA_PARSER_STATES_SPECIFIC
