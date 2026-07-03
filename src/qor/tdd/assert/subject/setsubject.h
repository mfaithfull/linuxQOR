// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#pragma once

#include "../proposition/hassizepropositions.h"
#include "../proposition/isemptypropositions.h"
#include "../proposition/isequaltopropositions.h"
#include "base.h"

namespace qor{ namespace assert{ namespace subject{

    template <typename C>
    class SetSubject : public virtual Base,
                    public proposition::IsEmptyPropositions<SetSubject<C>, C>,
                    public proposition::IsEqualToPropositions<SetSubject<C>, C>,
                    public proposition::HasSizePropositions<SetSubject<C>, C> 
    {
        const C& d_value;

    public:
        SetSubject(const bool failOnError, const char* file, int line, const C& v)
        : Base(failOnError, file, line), d_value(v) {}

    protected:
        const C* getValue() const override { return &d_value; }
    };

}}}//qor::assert::subject
