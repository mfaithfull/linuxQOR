// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_REQUEST_PROTOCOLTOKEN
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_REQUEST_PROTOCOLTOKEN

#include "src/components/data/parser/state.h"
#include "src/components/data/parser/states/rfc5234.h"
#include "src/components/data/parser/states/oneormore.h"
#include "src/components/data/parser/states/optional.h"
#include "src/components/data/parser/nodes/char.h"
#include "../tokendefs.h"
#include "../tokentoken.h"
#include "../nodes/protocolnode.h"
#include "protocolnametoken.h"

namespace qor { namespace components { namespace protocols { namespace http {

    //protocol         = protocol-name ["/" protocol-version]
    class qor_pp_module_interface(QOR_HTTP) protocol : public qor::components::parser::Sequence
    {
    public:

        protocol(qor::components::parser::Parser* parser);
        virtual ~protocol();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    };

}}}}//qor::components::protocols::http

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_REQUEST_PROTOCOLTOKEN