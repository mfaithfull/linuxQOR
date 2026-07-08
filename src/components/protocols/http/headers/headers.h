// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_HEADERS
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_HEADERS

#include <string>
#include <unordered_map>
#include "caseignore.h"

namespace qor { namespace components { namespace protocols { namespace http {

    using Headers = std::unordered_multimap<std::string, std::string, case_ignore::hash, case_ignore::equal_to>;

}}}}//qor::components::protocols::http

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_HEADERS
