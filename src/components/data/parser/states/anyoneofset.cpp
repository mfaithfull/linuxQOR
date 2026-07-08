// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/platform/compiler/compiler.h"
#include "anyoneofset.h"
#include "../context.h"
#include "../parser.h"
#include "src/qor/flyers/log/debug.h"

namespace qor { namespace components { namespace parser {

    //Matches one thing which is the first in the given set that matches. There can be only one. No matches in the set is an error
    AnyOneOfSet::AnyOneOfSet(Parser* parser, std::vector<ref_of<ParserState>::type>* set, uint64_t token) : ParserState(parser,token),
        m_set(set)
    {
        Enter = [this]()
        {
            Prepare();
            m_result.length = 0;
            m_it = m_set->begin();
            m_index = 0;
            if (m_it != m_set->end()) {
                log::debug("Trying to match first of a set of {0} parser states.", m_set->size());
                Workflow()->PushState((*m_it).AsRef<workflow::State>());
            } else {
                m_result.code = Result::FAILURE;
                m_result.length = 0;
            }
        };

        Resume = [this]()
        {
            auto parserState = (*m_it);
            if(parserState->m_result.code == Result::SUCCESS)
            {
                log::debug("Matched {0} of a set of {1} parser states.", m_index, m_set->size());
                m_result.first = parserState->m_result.first;
                m_result.m_position = parserState->m_result.m_position;
                m_result.length = parserState->m_result.length;
                m_result.code = Result::SUCCESS;
                m_result.token = m_token;
                Workflow()->PopState();
            }
            else
            {
                m_it++;
                m_index++;
                if(m_it == m_set->end())
                {
                    log::debug("Failed to match any of a set of {0} parser states.", m_set->size());
                    m_result.m_position = parserState->m_result.m_position;
                    m_result.length = 0;
                    m_result.code = Result::FAILURE;
                    Workflow()->PopState();
                }
                else
                {
                    log::debug("Trying to match {0} of a set of {1} parser states.", m_index, m_set->size());
                    Workflow()->PushState((*m_it));
                }
            }
        };
    }

    AnyOneOfSet::~AnyOneOfSet()
    {
        delete m_set;//TODO: Use a ref so this isn't necessary
    }

}}}//qor::components::parser
