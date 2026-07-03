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
    class IsEqualToPropositions : public virtual subject::Base 
    {
    protected:
        virtual const U* getValue() const = 0;

    public:
        T& isEqualTo(const U& other) 
        {
            if (!(*getValue() == other)) 
            {
            util::FailMessage::create()
                .file(getFile())
                .line(getLine())
                .fact(" not equal to", other)
                .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isNotEqualTo(const U& other) 
        {
            if (*getValue() == other) 
            {
            util::FailMessage::create()
                .file(getFile())
                .line(getLine())
                .fact(" not equal to", other)
                .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }
    };

}}}//qor::assert::proposition
