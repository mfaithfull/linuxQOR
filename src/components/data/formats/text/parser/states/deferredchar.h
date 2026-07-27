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

        deferredchar(Parser* parser, textToken type) : ParserState(parser)
        {
            SetCharType(type);            
            Init();
        }

        deferredchar(Parser* parser) : ParserState(parser)
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
                        GetParser()->Diagnostic();
                        m_p = CharacterType(GetParser());
                    }
                    GetParser()->PushState(m_p);
                };

            Resume = [this]()
                {
                    GetParser()->PopState();
                };

            Leave = [this]()
                {
                    if (m_p.IsNotNull())
                    {
                        m_result = m_p->m_result;
                    }
                };
        }

        ref_of<ParserState>::type CharacterType(Parser* parser)
        {
            if(m_charType == textToken::bom)
            {
                auto node = parser->PopNode();
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
                        parser->PushNode(node);//It's not a BOMNode so put it back where we found it.
                    }
                    else
                    {
                        m_charType = bomNode->GetBOMType();
                    }
                }
            }
            switch(m_charType)
            {
                case textToken::utf8BOM:
                return new_ref<utf8char>(parser);
                case textToken::utf16BEBOM:
                return new_ref<utf16BEchar>(parser);
                case textToken::utf32BEBOM:
                return new_ref<utf32BEchar>(parser);
                case textToken::utf32LEBOM:
                return new_ref<utf32LEchar>(parser);
                case textToken::utf16LEBOM:
                return new_ref<utf16LEchar>(parser);
                default:
                return new_ref<utf8char>(parser);
            }
        }

        ref_of<ParserState>::type m_p;
        textToken m_charType{textToken::bom};
    };

}}}}//qor::data::parser::text

#endif//QOR_PP_H_DATA_TEXT_PARSER_STATES_DEFERREDCHAR