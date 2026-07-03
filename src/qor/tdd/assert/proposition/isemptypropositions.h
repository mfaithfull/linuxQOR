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
    class IsEmptyPropositions : public virtual qor::assert::subject::Base 
    {
        protected:
        virtual const U* getValue() const = 0;

        public:
        T& isEmpty() 
        {
            if (!getValue()->empty()) 
            {
                util::FailMessage::create()
                .file(getFile())
                .line(getLine())
                .fact("map to be empty")
                .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isNotEmpty() 
        {
            if (getValue()->empty()) 
            {
                util::FailMessage::create()
                .file(getFile())
                .line(getLine())
                .fact("map to not be empty")
                .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }
    };

}}}//qor::assert::proposition