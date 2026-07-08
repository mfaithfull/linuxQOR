// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTTOKEN
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTTOKEN

#include "src/components/data/parser/state.h"
#include "src/components/data/parser/states/rfc5234.h"
#include "tokendefs.h"
#include "src/components/data/parser/states/oneormore.h"
#include "src/components/data/parser/nodes/char.h"
#include "request_linetoken.h"
#include "field/field_linetoken.h"

namespace qor { namespace components { namespace protocols { namespace http {

    class qor_pp_module_interface(QOR_HTTP) request : public qor::components::parser::Sequence
    {
    public:

        request(qor::components::parser::Parser* parser);
        virtual ~request();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    };

}}}}//qor::components::protocols::http

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTTOKEN