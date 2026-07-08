// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_PROGRESS
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_PROGRESS

#include <functional>

using DownloadProgress = std::function<bool(size_t current, size_t total)>;
using UploadProgress = std::function<bool(size_t current, size_t total)>;

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_PROGRESS