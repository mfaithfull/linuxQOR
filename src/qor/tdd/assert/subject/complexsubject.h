// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license
#pragma once

#include <complex>

#include "../proposition/isequaltopropositions.h"
#include "../proposition/isinpropositions.h"
#include "base.h"

namespace qor{ namespace assert{ namespace subject{

    template <typename T>
    class ComplexSubject : public virtual Base,
        public proposition::IsEqualToPropositions<ComplexSubject<T>, std::complex<T>>,
        public proposition::IsInPropositions<ComplexSubject<T>, std::complex<T>> 
    {
        const std::complex<T> d_value;

    protected:

        const std::complex<T>* getValue() const override { return &d_value; }

    public:

        ComplexSubject(const bool failOnError, const char* file, int line, const std::complex<T> v) : Base(failOnError, file, line), d_value(v) {}
    };

}}}//qor::assert::subject