// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_STATES_DEFERRED
#define QOR_PP_H_DATA_PARSER_STATES_DEFERRED

#include "../state.h"

namespace qor { namespace data { namespace parser {

    template<class T>
    class deferred : public ParserState
    {
    public:

        deferred(Parser* parser, uint64_t token) : ParserState(parser), m_token(token)
        {
            Enter = [this]()
                {
                    std::vector< ref_of<fastflow::Step>::type >& stateCache = GetParser()->GetStateCache(m_token);
                    if(stateCache.empty())
                    {
                        m_p = new_ref<T>(GetParser());
                    }
                    else
                    {
                        m_p = stateCache.back();                            
                        stateCache.pop_back();
                    }   
                    m_p->Enter();                 
                    //if (m_p.IsNotNull())
                    {                        
                        m_result = m_p->m_result;
                    }
                };

            Resume = [this]()
                {
                    //if (m_p.IsNotNull())
                    {
                        m_result = m_p->m_result;
                        m_p->Resume();
                        //if (m_p.IsNotNull())
                        {
                            m_result = m_p->m_result;
                        }
                    }
                };

            Leave = [this]()
                {
                    //if (m_p.IsNotNull())
                    {
                        m_p->Leave();
                        if (m_p.IsNotNull())
                        {
                            m_result = m_p->m_result;
                            std::vector< ref_of<fastflow::Step>::type >& stateCache = GetParser()->GetStateCache(m_token);
                            stateCache.emplace_back(m_p);
                        }
                    }
                };
        }

        virtual ~deferred()
        {
        }

    private:

        ref_of<T>::type m_p;
        uint64_t m_token;
    };

}}}//qor::data::parser

#endif//QOR_PP_H_DATA_PARSER_STATES_DEFERRED