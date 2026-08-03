// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright 2026 Ilya Korolev
// Licensed under the Apache License, Version 2.0
// SPDX-License-Identifier: Apache-2.0

#include "src/configuration/configuration.h"
#include "src/qor/tdd/tdd.h"
#include "../contract_test_types.h"

#include "src/framework/data/contract/adapters/schema.h"

#include <cassert>

using namespace qor;
using namespace contract_tests;

qor_pp_test_case(test_base_flattening) 
{
    static_assert(contract::field_count<RequestEvent>() == 3);

    RequestEvent event;
    CountingAdapter inherited_adapter;
    contract::visit(event, inherited_adapter);

    assert(inherited_adapter.fields == 3);
    assert(inherited_adapter.id_sum == 3104);
    assert((contract::field_at<0, RequestEvent>().get(event) == 7));

    contract::field_at<1, RequestEvent>().set(event, 22);
    assert(event.service == 22);

    assert(debug_string(event) == "timestamp=7, service=22, user_id=42");
    assert(contract::adapters::schema_string<RequestEvent>() == "1001 timestamp\n1002 service\n1101 user_id");

    static_assert(contract::field_count<RoutedEvent>() == 4);

    RoutedEvent routed;
    CountingAdapter routed_adapter;
    contract::visit(routed, routed_adapter);

    assert(routed_adapter.fields == 4);
    assert(routed_adapter.id_sum == 7005);

    contract::field_at<2, RoutedEvent>().set(routed, 55);
    assert(routed.trace_id == 55);

    assert(debug_string(routed) == "timestamp=7, service=11, trace_id=55, route_id=9");
    assert(contract::adapters::schema_string<RoutedEvent>() == "1001 timestamp\n1002 service\n2001 trace_id\n3001 route_id");

}
