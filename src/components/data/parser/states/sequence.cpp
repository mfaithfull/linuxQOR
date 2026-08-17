// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/platform/compiler/compiler.h"
#include "src/qor/flyers/log/debug.h"
#include "sequence.h"
#include "../context.h"
#include "../parser.h"

namespace qor { namespace data { namespace parser {

    //Matches a sequence beggining with head and followed by tail, which may itself be a sequence
    Sequence::Sequence(Parser* parser, ref_of<ParserState>::type head, ref_of<ParserState>::type tail, uint64_t token) : ParserState(parser, token),
        m_internalState(0), m_head(head), m_tail(tail)
    {
        m_internalState = 0;
        Enter = [this]()
            {
                Prepare();
                if(m_internalState == 0)                
                {
                    //log::debug("Trying Sequence head.");
                    Workflow()->PushStep(m_head);
                }
                else if(m_internalState == 1)
                {
                    //log::debug("Trying Sequence tail.");
                    Workflow()->PushStep(m_tail);
                }
            };

        Resume = [this]()
            {
                switch (m_internalState)
                {
                case 0://head
                    if (m_head->m_result.code == Result::FAILURE)
                    {
                        //log::debug("Sequence failed at head.");
                        m_result.code = m_head->m_result.code;
                        m_result.length = 0;
                        m_result.m_position = m_head->m_result.m_position;
                        Workflow()->PopStep();
                    }
                    else if (m_head->m_result.code == Result::SUCCESS)
                    {
                        //log::debug("Found Sequence head.");
                        m_result.first = m_head->m_result.first;
                        m_result.length = m_head->m_result.length;
                        m_result.m_position = m_head->m_result.m_position;
                        m_internalState = 1;
                        Workflow()->PushStep(m_tail);
                    }
                    else
                    {                        
                        //log::debug("Sequence ran out of data.");
                        if (GetParser()->IsFinal())
                        {
                            Workflow()->PopStep();
                        }                        
                        return;
                    }
                    break;
                case 1://tail
                    
                    if (m_tail->m_result.code == Result::SUCCESS)
                    {
                        //log::debug("Sequence found.");
                        m_result.code = Result::SUCCESS;
                        m_result.length = m_head->m_result.length + m_tail->m_result.length;
                        m_result.token = m_token;
                        m_internalState = 0;
                        Workflow()->PopStep();
                    }
                    else if (m_tail->m_result.code == Result::FAILURE)
                    {
                        //log::debug("Sequence failed at tail.");
                        m_result.code = Result::FAILURE;
                        m_result.length = 0;
                        m_internalState = 0;
                        Workflow()->PopStep();
                    }
                    else
                    {
                        //log::debug("Sequence tail ran out of data.");
                        if (GetParser()->IsFinal())//
                        {
                            m_result.code = Result::FAILURE;
                            m_internalState = 0;
                            Workflow()->PopStep();
                        }
                        else
                        {
                            return;
                        }
                    }
                    break;
                }
            };
    }

    void Sequence::Prepare()
    {
        //log::debug("Looking for a sequence...");
    }
    Sequence::~Sequence() = default;
    
}}}//qor::data::parser
