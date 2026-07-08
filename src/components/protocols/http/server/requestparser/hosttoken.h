// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_HOSTTOKEN
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_HOSTTOKEN

#include "src/components/data/parser/state.h"
#include "src/components/data/parser/states/rfc5234.h"
#include "tokendefs.h"
#include "src/components/data/parser/states/oneormore.h"
#include "src/components/data/parser/nodes/char.h"
#include "nodes/hostnode.h"
#include "ipv4addresstoken.h"
#include "reg_nametoken.h"

namespace qor { namespace components { namespace protocols { namespace http {

    //IP-literal / IPv4address / reg-name
    class qor_pp_module_interface(QOR_HTTP) host : public qor::components::parser::AnyOneOfSet
    {
    public:
        host(qor::components::parser::Parser* parser);
        virtual ~host();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}}//qor::components::protocols::http

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTPARSER_HOSTTOKEN