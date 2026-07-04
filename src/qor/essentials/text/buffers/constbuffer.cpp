// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <stdexcept>
#include <format>
#include "src/qor/essentials/reporting/ierrorreporting.h"
#include "constbuffer.h"

namespace qor{ namespace text {

    void OutOfRangeError(size_t index, size_t length, size_t elementSize, const void* bufferAddress)
    {
        IErrorReporting().continuable(std::vformat("Buffer index out of range accesssing index {0} of {1} elements of size {2} bytes in buffer at {3:p}", std::make_format_args(index, length, elementSize, bufferAddress)));
    }

}}//qor::text

