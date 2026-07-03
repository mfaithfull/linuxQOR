// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license
#pragma once

#include "../proposition/comparingpropositions.h"
#include "../proposition/isequaltopropositions.h"
#include "../proposition/isinpropositions.h"
#include "../proposition/stdfloatingpropositions.h"
#include "base.h"

namespace qor{ namespace assert{ namespace subject{

template <typename T>
class FloatingSubject : public virtual Base,
                        public proposition::FloatingPropositions<FloatingSubject<T>, T>,
                        public proposition::IsInPropositions<FloatingSubject<T>, T>,
                        public proposition::ComparisonPropositions<FloatingSubject<T>, T> 
{
    const T d_value;
    T d_tolerance;

protected:
    const T* getValue() const override 
    { 
        return &d_value; 
    }

    const T* getTolerance() const override 
    { 
        return &d_tolerance; 
    }

public:

    FloatingSubject(const bool failOnError, const char* file, int line, const T v) : Base(failOnError, file, line), d_value(v), d_tolerance(std::numeric_limits<T>::epsilon()) {}

    FloatingSubject& isWithin(const T& tolerance) 
    {
        d_tolerance = tolerance;
        return *this;
    }
};

}}}//qor::assert::subject