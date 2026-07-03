// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#pragma once

#include <memory.h>

#include "../adl.h"
#include "../assert.h"
#include "../subject/base.h"
#include "../util/failmessage.h"

namespace qor{ namespace assert{ namespace proposition{

    template <typename U>
    class HasValueThatPropositions : public virtual subject::Base 
    {
    protected:

        virtual const U* getValue() const = 0;

    public:

        auto hasValueThat() 
        {
            return assert_that_internal(
            qor::assert::Adl(), getFailOnError(), getFile(), getLine(), **getValue());
        }
    };

}}}//qor::assert::proposition
