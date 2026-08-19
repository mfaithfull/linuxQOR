// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSESINK
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSESINK

#include <string>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/data/pipeline/sink.h"
#include "../../response/response.h"
#include "../responseparser/responseparser.h"
#include "src/components/data/parser/context.h"

namespace qor { namespace components { namespace protocols { namespace http {

    class qor_pp_module_interface(QOR_HTTP) HTTPSink : public pipeline::Sink
    {
    public:

        HTTPSink();
        virtual ~HTTPSink() = default;

        virtual bool Write(size_t& unitsWritten, size_t unitsToWrite = 1 );
        ref_of<HTTPResponse>::type GetResponse();

    protected:

        virtual size_t Write(byte* data, size_t bytesToWrite);
        bool Pull(size_t& unitsWritten, size_t unitsToWrite);
        bool Push(size_t& unitsWritten, size_t unitsToWrite);
        
        ref_of<data::AbstractDataContext>::type m_context;
        data::Parser m_parser;
        ref_of<HTTPResponse>::type m_response;
    };

}}}}//qor::components::protocols::http

#endif// QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSESINK