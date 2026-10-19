// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/log/debug.h"
#include "sequence.h"
#include "begin_sequence.h"
#include "../_4/value.h"
#include "../_4/value_separator.h"
#include "../_2/end_sequence.h"
#include "../../nodes/sequence.h"
#include "../../nodes/value.h"

namespace qor { namespace protocols { namespace yaml {

    //sequence = begin-sequence [ value *( value-separator value ) ] end-sequence
    sequence::sequence(Parser* parser) :
        Sequence(parser,
            new_ref<begin_sequence>(parser),
            new_ref<Sequence>(parser,
                new_ref<Optional>(parser,
                    new_ref<Sequence>(parser,
                        new_ref<value>(parser),
                        new_ref<ZeroOrMore>(parser,
                            new_ref<Sequence>(parser,
                                new_ref<value_separator>(parser),
                                new_ref<value>(parser)
                            )
                        )
                    )
                ),
                new_ref<end_sequence>(parser)
            ),static_cast<uint64_t>(yamlToken::sequence)),
            m_parser(parser)
    { }

    sequence::~sequence() noexcept = default;

    void sequence::Prepare()
    {
        log::debug("Looking for a Sequence...");
        m_parser->PushNode(new_ref<SequenceNode>().AsRef<Node>());
    }

    void sequence::Emit()
    {
        log::debug("Emitting a Sequence:");
        std::vector<ref_of<ValueNode>::type> values;
        auto node = m_parser->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();

            if(token == static_cast<uint64_t>(yamlToken::value))
            {
                auto valueNode = node.AsRef<ValueNode>();
                values.push_back(valueNode);
            }
            else
            {
                auto f = yamlTokenNames.find((yamlToken)token);
                std::string tokenName;
                if(f != yamlTokenNames.end())
                {
                    tokenName = f->second;
                }
                continuable("Unexpected: {0}", tokenName);
            }
            node = m_parser->PopNode();
        }

        if(node.IsNotNull())
        {
            for(auto it = values.rbegin(); it != values.rend(); ++it)
            {
                node.AsRef<SequenceNode>()->GetObject()->m_values.push_back((*it)->GetObject());
            }
            m_parser->PushNode(node);
        }
    }

    void sequence::Fail()
    {
        //log::debug("...Didn't find a Sequence.");
        uint64_t topToken = m_parser->TopNode()->GetToken();
        if(topToken == m_token)
        {
            m_parser->PopNode();
        }
    }

}}}//qor::protocols::yaml
