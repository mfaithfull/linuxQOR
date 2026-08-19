// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSENODE
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSENODE

#include <string>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/components/data/parser/parser.h"
#include "responseparser.h"
#include "../../response/response.h"

namespace qor { namespace components { namespace protocols { namespace http { namespace response {

    class ResponseNode : public data::parser::NodeAdapter<HTTPResponse>
    {
    public:

        ResponseNode() : data::parser::NodeAdapter<HTTPResponse>(static_cast<uint64_t>(httpResponseToken::response))
        {
        }

        ResponseNode(qor::ref_of<HTTPResponse>::type response) : data::parser::NodeAdapter<HTTPResponse>(response, static_cast<uint64_t>(httpResponseToken::response))
        {
        }

        virtual ~ResponseNode() = default;

    };

}}}}}//qor::components::protocols::http::response

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSENODE
