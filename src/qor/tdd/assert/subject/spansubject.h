// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license
#pragma once

#if __cpp_lib_span >= 202002L

#include <span>

#include "../proposition/hassizepropositions.h"
#include "../proposition/isemptypropositions.h"
#include "base.h"

namespace qor{ namespace assert{ namespace subject{

    template <typename T>
    class SpanSubject : public virtual Base, public proposition::HasSizePropositions<SpanSubject<T>, T>, public proposition::IsEmptyPropositions<SpanSubject<T>, T> 
    {
        const T d_value;

    protected:

        const T* getValue() const override 
        { 
            return &d_value; 
        }

    public:

        SpanSubject(const bool failOnError, const char* file, int line, const T& v): Base(failOnError, file, line), d_value(v) {}
    };

}}}//qor::assert::subject

#endif