// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_NODE
#define QOR_PP_H_COMPONENTS_PARSER_NODE

#include <cstdint>
#include <string>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"

namespace qor { namespace components { namespace parser {

    class Node
    {
    public:

        Node(uint64_t token) : m_token(token){ }

        virtual ~Node() = default;

        uint64_t GetToken() const
        {
            return m_token;
        }

        virtual std::string ToString() const {return "<anonymous node>";}
        virtual void Diagnostic(){}

    private:
        
        uint64_t m_token;        
    };

    template<class T>
    class NodeAdapter : public Node
    {
    public:

        NodeAdapter(uint64_t token) : Node(token)
        {
            m_t = qor::new_ref<T>();
        }

        NodeAdapter(qor::ref_of<T>::type response, uint64_t token) : Node(token)
        {
            m_t = response;
        }

        virtual ~NodeAdapter() = default;

        typename ref_of<T>::type GetObject() const
        {
            return m_t;
        }

    protected:

        typename ref_of<T>::type m_t;
    };

}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_NODE