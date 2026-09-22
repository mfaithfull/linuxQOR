// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <string>
#include <stdexcept>
#include <iostream>
#include <type_traits>

#include "src/qor/tdd/tdd.h"
#include "src/components/protocols/uri/uri.h"

qor_pp_test_case(uri_basics)
{
    qor::data::URI arui("http://www.mydomain.egg");
}
