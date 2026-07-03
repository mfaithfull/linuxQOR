// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#pragma once

#include <memory.h>
#include "../subject/base.h"
#include "../subject/integralsubject.h"
#include "../util/failmessage.h"

namespace qor{ namespace assert{ namespace proposition{

    template <typename T, typename U>
    class HasSizePropositions : public virtual subject::Base 
    {
    protected:
        virtual const U* getValue() const = 0;

    public:
        T& hasSize(std::size_t size) 
        {
            if (getValue()->size() != size) 
            {
                util::FailMessage::create()
                .file(getFile())
                .line(getLine())
                .fact("size is equal to", size)
                .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        subject::IntegralSubject<std::size_t> hasSizeThat() 
        {
            return subject::IntegralSubject<std::size_t>(
            getFailOnError(), getFile(), getLine(), (std::size_t)getValue()->size());
        }

    };

}}}//qor::assert::proposition
