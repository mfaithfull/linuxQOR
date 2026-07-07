// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_REQUESTNODE
#define QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_REQUESTNODE

#include <string>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/components/data/parser/parser.h"
#include "echorequestparser.h"
#include "echorequest.h"

namespace qor { namespace components { namespace protocols { namespace echo {

    class RequestNode : public qor::components::parser::NodeAdapter<EchoRequest>
    {
    public:

        RequestNode() : qor::components::parser::NodeAdapter<EchoRequest>(static_cast<uint64_t>(echoRequestToken::request))
        { }

        RequestNode(qor::ref_of<EchoRequest>::type request) : qor::components::parser::NodeAdapter<EchoRequest>(request, static_cast<uint64_t>(echoRequestToken::request))
        { }

        virtual ~RequestNode() = default;

    };
    
}}}}//qor::components::protocols::echo

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_REQUESTNODE
