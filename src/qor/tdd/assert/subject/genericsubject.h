// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#pragma once

#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "../proposition/isequaltopropositions.h"
#include "base.h"

namespace qor{ namespace assert{ namespace subject{

    template <typename T>
    class GenericSubject : public proposition::IsEqualToPropositions<GenericSubject<T>, T> 
    {
        const T& d_value;

    protected:

        const T* getValue() const override 
        { 
            return &d_value; 
        }

    public:
        
        GenericSubject(const bool failOnError, const char* file, int line, const T& v) : Base(failOnError, file, line), d_value(v) {}
    };

}}}//qor::assert::subject