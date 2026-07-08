// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUEST_CONTENT
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUEST_CONTENT

#include <functional>

namespace qor { namespace components { namespace protocols { namespace http {

    using ContentProvider = std::function<bool(size_t offset, size_t length/*, DataSink &sink*/)>;
    using ContentReceiverWithProgress = std::function<bool(const char *data, size_t data_length, size_t offset, size_t total_length)>;
    using ContentReceiver = std::function<bool(const char *data, size_t data_length)>;

}}}}//qor::components::protocols::http

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUEST_CONTENT