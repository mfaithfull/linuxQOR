// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_HELPERS
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_HELPERS

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/components/data/parser/state.h"
#include "src/components/data/parser/node.h"
#include "tokens.h"

namespace qor { namespace protocols { namespace yaml {

    std::string GetTokenName(uint64_t token)
    {
        auto f = yamlTokenNames.find((yamlToken)token);
        std::string tokenName;
        if(f != yamlTokenNames.end())
        {
            tokenName = f->second;
            return tokenName;
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
        return tokenName;
    }

    template<class NodeT>
    void StandardPrepare(qor::data::parser::ParserState& state)
    {
        std::string tokenName = GetTokenName(state.GetToken());
        log::debug("Looking for a {0}...", tokenName);
        state.GetParser()->PushNode(new_ref<NodeT>().template AsRef<qor::data::parser::Node>());
    }

    template< class NodeT >
    void StandardFail(qor::data::parser::ParserState& state)
    {
        std::string tokenName = GetTokenName(state.GetToken());
        log::debug("...Didn't find a {0}.", tokenName);
        qor::ref_of<qor::data::parser::Node>::type node = state.GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != state.GetToken())
        {
            state.GetParser()->PushNode(node);
        }
    }

    template<class NodeT>
    void TrivialEmit(qor::data::parser::ParserState& state)
    {
        std::string tokenName = GetTokenName(state.GetToken());
        log::debug("Emitting a {0}.", tokenName);
        state.GetParser()->PushNode(new_ref<NodeT>());
    }

    template<class NodeT>
    void SimpleEmit(qor::data::parser::ParserState& state)
    {
        std::string thisTokenName = GetTokenName(state.GetToken());
        log::debug("Building {0}", thisTokenName);

        auto node = state.GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != state.GetToken())
        {
            uint64_t token = node->GetToken();
            std::string tokenName = GetTokenName(token);
            log::debug("Consuming {0}", tokenName);
            node = state.GetParser()->PopNode();
        };

        if(node.IsNotNull())
        {
            state.GetParser()->PushNode(new_ref<NodeT>());
            log::debug("Emitted {0}", thisTokenName);
        }
    }

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_HELPERS