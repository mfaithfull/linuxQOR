// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_REQUEST_LINETOKEN
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_REQUEST_LINETOKEN

#include "src/components/data/parser/state.h"
#include "src/components/data/parser/states/rfc5234.h"
#include "tokendefs.h"
#include "src/components/data/parser/states/oneormore.h"
#include "src/components/data/parser/nodes/char.h"
#include "nodes/request_linenode.h"
#include "targettoken.h"
#include "tokentoken.h"
#include "protocol/protocoltoken.h"

namespace qor { namespace components { namespace protocols { namespace http {

    class qor_pp_module_interface(QOR_HTTP) request_line : public parser::Sequence
    {
    public:
        request_line(parser::Parser* parser);
        virtual ~request_line();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    };

}}}}//qor::components::protocols::http

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_REQUEST_LINETOKEN