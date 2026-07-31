// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_TEXT_PARSER_STATES_DEFERREDCHAR
#define QOR_PP_H_DATA_TEXT_PARSER_STATES_DEFERREDCHAR

#include "src/components/data/parser/state.h"
#include "src/components/data/parser/result.h"
#include "utf8/utf8char.h"
#include "utf16/utf16bechar.h"
#include "utf16/utf16lechar.h"
#include "utf32/utf32bechar.h"
#include "utf32/utf32lechar.h"
#include "../nodes/bomnode.h"

namespace qor { namespace data { namespace parser { namespace text {
    
    class deferredchar : public ParserState
    {
    public:

        deferredchar(Parser* parser, textToken type) : ParserState(parser), m_parser(parser)
        {
            SetCharType(type);            
            Init();
        }

        deferredchar(Parser* parser) : ParserState(parser), m_parser(parser)
        {
            Init();
        }

        virtual ~deferredchar() = default;

        textToken GetCharType()
        {
            return m_charType;
        }

        void SetCharType(textToken type)
        {
            m_charType = type;
        }

    private:

        void Init()
        {
            Enter = [this]()
                {
                    if(m_p.IsNull())
                    {
                        //m_parser->Diagnostic();
                        m_p = CharacterType();
                    }
                    m_parser->PushStep(m_p);
                };

            Resume = [this]()
                {
                    m_parser->PopStep();
                };

            Leave = [this]()
                {
                    if (m_p.IsNotNull())
                    {
                        m_result = m_p->m_result;
                    }
                };
        }

        ref_of<ParserState>::type CharacterType()
        {
            if(m_charType == textToken::bom)
            {
                auto node = m_parser->TopNode();
                if(node.IsNull())
                {
                    m_charType = textToken::utf8BOM;                
                }
                else 
                {
                    auto bomNode = node.AsRef<BOMNode>();
                    if(bomNode.IsNull())
                    {
                        m_charType = textToken::utf8BOM;
                    }
                    else
                    {
                        m_charType = bomNode->GetBOMType();
                        m_parser->PopNode();
                    }
                }
            }
            switch(m_charType)
            {
                case textToken::utf8BOM:
                return new_ref<utf8char>(m_parser);
                case textToken::utf16BEBOM:
                return new_ref<utf16BEchar>(m_parser);
                case textToken::utf32BEBOM:
                return new_ref<utf32BEchar>(m_parser);
                case textToken::utf32LEBOM:
                return new_ref<utf32LEchar>(m_parser);
                case textToken::utf16LEBOM:
                return new_ref<utf16LEchar>(m_parser);
                default:
                return new_ref<utf8char>(m_parser);
            }
        }

        ref_of<ParserState>::type m_p;
        textToken m_charType{textToken::bom};
        Parser* m_parser;
    };

}}}}//qor::data::parser::text

namespace qor{
    qor_pp_declare_source_of(data::parser::text::deferredchar, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::text::deferredchar>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_TEXT_PARSER_STATES_DEFERREDCHAR