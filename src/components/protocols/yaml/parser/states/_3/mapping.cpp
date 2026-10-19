// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include <iostream>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "mapping.h"
#include "../_1/begin_mapping.h"
#include "../_2/end_mapping.h"
#include "../_3/pair.h"
#include "../_4/value_separator.h"
#include "../../nodes/mapping.h"
#include "../../nodes/pair.h"

namespace qor { namespace protocols { namespace yaml {

    //mapping = begin-mapping [ pair *( value-separator pair ) ] end-mapping
    mapping::mapping(Parser* parser) :
        Sequence(parser,
            new_ref<begin_mapping>(parser),
            new_ref<Sequence>(parser,
                new_ref<Optional>(parser,
                    new_ref<Sequence>(parser,
                        new_ref<pair>(parser),
                        new_ref<ZeroOrMore>(parser,
                            new_ref<Sequence>(parser,
                                new_ref<value_separator>(parser),
                                new_ref<pair>(parser)
                            )
                        )
                    )
                ),
                new_ref<end_mapping>(parser)
            ),
        static_cast<uint64_t>(yamlToken::mapping)){ }

    mapping::~mapping() noexcept = default;

    void mapping::Prepare()
    {
        log::debug("Looking for a Mapping...");
        GetParser()->PushNode(new_ref<MappingNode>());
    }

    void mapping::Emit()
    {
        log::debug("Emitting a Mapping");
        std::vector<ref_of<PairNode>::type> pairs;
        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();

            if(token == static_cast<uint64_t>(yamlToken::pair))
            {
                auto pairNode = node.AsRef<PairNode>();
                pairs.push_back(pairNode);
            }
            else
            {
                auto f = yamlTokenNames.find((yamlToken)token);
                std::string tokenName;
                if(f != yamlTokenNames.end())
                {
                    tokenName = f->second;
                }
                continuable("Unexpected {0}", tokenName);
            }
            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            for(auto it = pairs.rbegin(); it != pairs.rend(); ++it)
            {
                node.AsRef<MappingNode>()->GetObject()->m_pairs.push_back((*it)->GetObject());
            }
            GetParser()->PushNode(node);
        }
    }

    void mapping::Fail()
    {
        //log::debug("...Didn't find a Mapping.");
        uint64_t topToken = m_parser->TopNode()->GetToken();
        if(topToken == m_token)
        {
            m_parser->PopNode();
        }
    }

}}}//qor::protocols::yaml
