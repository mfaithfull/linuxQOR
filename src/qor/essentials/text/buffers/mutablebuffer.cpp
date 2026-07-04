// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <stdexcept>
#include <format>
#include "src/qor/essentials/reporting/ierrorreporting.h"
#include "mutablebuffer.h"

namespace qor{ namespace text {

    void BufferFreeWhileSharedError(const void* bufferAddress, unsigned short refCount)
    {        
        IErrorReporting().serious(std::vformat("Attempted to free buffer at {0:p} while reference count == {1}.", std::make_format_args(bufferAddress, refCount)));
    }

    void BufferOverrunError(const void* bufferAddress, unsigned short headerRefCount, unsigned short footerRefCount)
    {
        IErrorReporting().serious(std::vformat("Buffer overrun detected on buffer at {0:p}. footer {1} does not match header {2}", std::make_format_args(bufferAddress, footerRefCount, headerRefCount)));
    }

}}//qor::text

