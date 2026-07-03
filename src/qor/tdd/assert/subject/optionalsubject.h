// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#pragma once

#include <optional>

#include "../proposition/isequaltopropositions.h"
#include "../proposition/valuecontainerpropositions.h"
#include "../proposition/hasvaluethatpropositions.h"
#include "base.h"

namespace qor{ namespace assert{ namespace subject{

    template <typename T>
    class OptionalSubject : public virtual Base,
                            public proposition::HasValueThatPropositions<T>,
                            public proposition::IsEqualToPropositions<OptionalSubject<T>, T>,
                            public proposition::ValueContainsPropositions<OptionalSubject<T>, T> 
    {
        const T d_value;

        protected:
        const T* getValue() const override { return &d_value; }

        public:
        OptionalSubject(const bool failOnError, const char* file, int line, const T& v)
            : Base(failOnError, file, line), d_value(v) {}

        OptionalSubject<T>& isPresent() 
        {
            if (!*getValue()) 
            {
                util::FailMessage::create()
                    .failOnError(getFailOnError())
                    .file(getFile())
                    .line(getLine())
                    .fact("Expected the optional to contain a value.")
                    .fact("The optional does not contain a value.");
            }
            return *this;
        }

        OptionalSubject<T>& isEmpty() 
        {
            if (*getValue()) 
            {
                util::FailMessage::create()
                    .failOnError(getFailOnError())
                    .file(getFile())
                    .line(getLine())
                    .expected("Expected the optional to be empty.")
                    .fact("Got", *getValue());
            }
            return *this;
        }
    };

}}}//qor::assert::subject
