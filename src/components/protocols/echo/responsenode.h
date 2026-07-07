// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_RESPONSENODE
#define QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_RESPONSENODE

#include <string>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/components/data/parser/parser.h"
#include "echoresponseparser.h"
#include "echoresponse.h"

namespace qor { namespace components { namespace protocols { namespace echo {

    class ResponseNode : public qor::components::parser::NodeAdapter<EchoResponse>
    {
    public:

        ResponseNode() : qor::components::parser::NodeAdapter<EchoResponse>(static_cast<uint64_t>(echoResponseToken::response))
        { }

        ResponseNode(qor::ref_of<EchoResponse>::type response) : qor::components::parser::NodeAdapter<EchoResponse>(response,static_cast<uint64_t>(echoResponseToken::response))
        { }

        virtual ~ResponseNode() = default;

    };
    
}}}}//qor::components::protocols::echo

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_RESPONSENODE
