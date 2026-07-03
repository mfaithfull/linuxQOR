// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#pragma once

#include <memory.h>
#include "../subject/base.h"
#include "../util/failmessage.h"

namespace qor{ namespace assert{ namespace proposition{

    template <typename T, typename U>
    class ValueContainsPropositions : public virtual subject::Base 
    {
    protected:
        virtual const U* getValue() const = 0;

    public:

        T& contains(const U& other) 
        {
            if (!(getValue()->value() == other)) 
            {
                util::FailMessage::create()
                .file(getFile())
                .line(getLine())
                .fact("contains ", other)
                .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& doesNotContain(const U& other) 
        {
            if (getValue()->value() == other) 
            {
                util::FailMessage::create()
                .file(getFile())
                .line(getLine())
                .fact("does not contain ", other)
                .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }
    };

}}}//qor::assert::proposition

