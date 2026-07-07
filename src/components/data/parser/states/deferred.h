// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_STATES_DEFERRED
#define QOR_PP_H_COMPONENTS_PARSER_STATES_DEFERRED

#include "../state.h"

namespace qor { namespace components { namespace parser {

    template<class T>
    class deferred : public ParserState
    {
    public:

        deferred(Parser* parser) : ParserState(parser)
        {
            Enter = [this]()
                {
                    m_p = new_ref<T>(GetParser());
                    m_p->Enter();
                    if (m_p.IsNotNull())
                    {
                        m_result = m_p->m_result;
                    }
                };

            Resume = [this]()
                {
                    if (m_p.IsNotNull())
                    {
                        m_result = m_p->m_result;
                        m_p->Resume();
                        if (m_p.IsNotNull())
                        {
                            m_result = m_p->m_result;
                        }
                    }
                };

            Suspend = [this]()
                {
                    if (m_p.IsNotNull())
                    {
                        m_p->Suspend();
                        if (m_p.IsNotNull())
                        {
                            m_result = m_p->m_result;
                        }
                    }
                };

            Leave = [this]()
                {
                    if (m_p.IsNotNull())
                    {
                        m_p->Leave();
                        if (m_p.IsNotNull())
                        {
                            m_result = m_p->m_result;
                        }
                    }
                };
        }

        virtual ~deferred() = default;

    private:

        ref_of<T>::type m_p;
    };

}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_STATES_DEFERRED