// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#pragma once

#include "../proposition/isequaltopropositions.h"
#include "base.h"
#include "../util/failmessage.h"

namespace qor{ namespace assert{ namespace subject{

    class BoolSubject : public virtual Base, public proposition::IsEqualToPropositions<BoolSubject, bool> 
    {
        const bool d_value;

    protected:

        const bool* getValue() const override { return &d_value; }

    public:

        BoolSubject(const bool failOnError, const char* file, int line, const bool v) : Base(failOnError, file, line), d_value(v) {}

        BoolSubject& isTrue() 
        {
            if (d_value != true) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("expected boolean to have the value of true")
                    .fact("value was ", d_value);
            }
            return *this;
        }

        BoolSubject& isFalse() 
        {
            if (d_value != false) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("expected boolean to have the value of false")
                    .fact("value was ", d_value);
            }
            return *this;
        }
    };

}}}//qor::assert::subject