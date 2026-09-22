// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "this.h"

namespace qor{ namespace par{

    thread_local this_contract* this_contract::tlsThisContract_ = nullptr;

    qor_pp_export this_contract*& GetCurrentWorkItem()
    {
        return this_contract::tlsThisContract_;
    }

}}//qor::par
