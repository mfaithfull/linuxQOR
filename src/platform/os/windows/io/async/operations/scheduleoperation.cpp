// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "scheduleoperation.h"
#include "../ioservice.h"
namespace qor { namespace io { namespace async { namespace win {

    void ScheduleOperation::await_suspend(std::coroutine_handle<> awaiter) noexcept
    {
        m_awaiter = awaiter;
        m_service.ScheduleImpl(this);
    }

}}}}//qor::io::async::win
