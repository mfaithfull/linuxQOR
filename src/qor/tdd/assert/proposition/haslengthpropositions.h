// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#pragma once

#include <limits>
#include <memory>

#include "../subject/base.h"
#include "../subject/integralsubject.h"
#include "../util/failmessage.h"

namespace qor{ namespace assert{ namespace proposition{

    template <typename T, typename U>
    class HasLengthPropositions : public virtual subject::Base 
    {
    protected:

        virtual const U* getValue() const = 0;

    public:

        T& hasLength(const std::size_t length) 
        {
            if (getValue()->length() != length) 
            {
                util::FailMessage::create()
                .file(getFile())
                .line(getLine())
                .fact("length is equal to", length)
                .fact("Got", getValue()->length());
            }
            return *dynamic_cast<T*>(this);
        }

        subject::IntegralSubject<std::size_t> hasLengthThat() 
        {
            return subject::IntegralSubject<std::size_t>(
            getFailOnError(), getFile(), getLine(), (std::size_t)getValue()->length());
        }
    };

}}}//qor::assert::proposition
