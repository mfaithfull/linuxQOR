// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#pragma once

#include <string_view>

#include "../proposition/hassizepropositions.h"
#include "../proposition/isemptypropositions.h"
#include "base.h"

namespace qor{ namespace assert{ namespace subject{

    template <typename T>
    class StringViewSubject : public virtual Base,
                            public proposition::HasLengthPropositions<StringViewSubject<T>, T>,
                            public proposition::HasSizePropositions<StringViewSubject<T>, T>,
                            public proposition::IsEmptyPropositions<StringViewSubject<T>, T> 
    {
        const T d_value;

    protected:
        const T* getValue() const override { return &d_value; }

    public:
        StringViewSubject(const bool failOnError, const char* file, int line, const T& v)
        : Base(failOnError, file, line), d_value(v) {}
    };

}}}//qor::assert::subject
