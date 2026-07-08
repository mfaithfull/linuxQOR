// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUEST_PARAMS
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUEST_PARAMS

#include <string>
#include <map>

namespace qor { namespace components { namespace protocols { namespace http {
    
    using Params = std::multimap<std::string, std::string>;

}}}}//qor::components::protocols::http


#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUEST_PARAMS