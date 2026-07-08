// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "escapesequence.h"
#include "../nodes/escapesequence.h"
#include "../nodes/character.h"

namespace qor { namespace components { namespace parser { namespace json {

    escapeSequence::escapeSequence(Parser* parser) : Sequence(parser, 
            new_ref<escape>(parser),
            new_ref<AnyOneOf>(parser, 
                new_ref<Specific>(parser, (byte)0x22, static_cast<uint64_t>(jsonToken::quotation_mark)),
                new_ref<AnyOneOf>(parser,
                    new_ref<Specific>(parser, (byte)0x5C),// '\\'
                    new_ref<AnyOneOf>(parser,
                        new_ref<Specific>(parser, (byte)0x2F),//  '/'
                        new_ref<AnyOneOf>(parser,
                            new_ref<Specific>(parser, (byte)0x62),// 'b'
                            new_ref<AnyOneOf>(parser,
                                new_ref<Specific>(parser, (byte)0x66),// 'f'
                                new_ref<AnyOneOf>(parser,
                                    new_ref<Specific>(parser, (byte)0x6E),// 'n'
                                    new_ref<AnyOneOf>(parser,
                                        new_ref<Specific>(parser, (byte)0x72),// 'r'
                                        new_ref<AnyOneOf>(parser,
                                            new_ref<Specific>(parser, (byte)0x74),// 't'
                                            new_ref<unicodeSequence>(parser)
                                        )
                                    )
                                )
                            )
                        )
                    )
                )
            ),static_cast<uint64_t>(jsonToken::escape_sequence)
        )
    { }

    escapeSequence::~escapeSequence() = default;

    void escapeSequence::Prepare()
    {
        log::debug("Looking for an Escape Sequence...");
        GetParser()->PushNode(new_ref<EscapeSequenceNode>());
    }

    void escapeSequence::Emit()
    {
        log::debug("Emitting an Escape Sequence");
        char cValue = 0;
        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();
            auto f = jsonTokenNames.find((jsonToken)token);
            std::string tokenName;
            if(f != jsonTokenNames.end())
            {
                tokenName = f->second;
            }
            else
            {
                auto g = tokenNames.find((eToken)token);
                if( g != tokenNames.end())
                {
                    tokenName = g->second;
                }
                else
                {
                    continuable("Unrecognized token {0}", token);
                }
            }
            node = GetParser()->PopNode();                
        };

        if(node.IsNotNull())
        {            
            log::debug("Escaped Sequence to: {0}", cValue);
            GetParser()->PushNode(new_ref<CharacterNode>(cValue));
        }
    }

    void escapeSequence::Fail()
    {
        log::debug("...Didn't find an Escape Sequence.");
        auto node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

}}}}//qor::components::parser::json
