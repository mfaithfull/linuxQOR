// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license
#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include "base.h"
#include "integralsubject.h"
#include "../util/checkutil.h"
#include "../util/failmessage.h"

namespace qor{ namespace assert{ namespace subject{

    template <typename T>
    class IteratorsSubject 
    {
        const char* d_file;
        const int d_line;
        const T d_begin;
        const T d_end;
        const bool d_failOnError;

        using value_t = typename std::iterator_traits<T>::value_type;

    public:
        IteratorsSubject(const bool failOnError, const char* file, int line, const T biter, const T eiter)
            : d_file(file), d_line(line), d_begin(biter), d_end(eiter), d_failOnError(failOnError) {}

        IteratorsSubject<T>& isEmpty() 
        {
            if (d_begin != d_end) 
            {
                util::FailMessage::create()
                    .file(d_file)
                    .line(d_line)
                    .fact("Expected the collection to be empty")
                    .fact("Got", d_begin, d_end);
            }
            return *this;
        }

        IteratorsSubject<T>& isNotEmpty() 
        {
            if (d_begin == d_end) 
            {
                util::FailMessage::create()
                    .file(d_file)
                    .line(d_line)
                    .fact("Expected the collection to not empty.")
                    .fact("Got an empty collection.");
            }
            return *this;
        }

        template <typename A>
        IteratorsSubject<T> isNotEqualTo(const A& ob, const A& oe) 
        {
            if (std::equal(d_begin, d_end, ob, oe)) 
            {
                util::FailMessage::create()
                    .file(d_file)
                    .line(d_line)
                    .fact("Expected the collection not to be equal to", ob, oe)
                    .fact("Got", d_begin, d_end);
            }
            return *this;
        }

        template <typename A>
        IteratorsSubject<T>& isEqualTo(const A& ob, const A& oe) 
        {
            if (!std::equal(d_begin, d_end, ob, oe)) 
            {
                util::FailMessage::create()
                    .file(d_file)
                    .line(d_line)
                    .fact("Expected the collection is equal to", ob, oe)
                    .fact("Got", d_begin, d_end);
            }
            return *this;
        }

        IteratorsSubject<T>& contains(value_t r) 
        {
            auto iter = std::find(d_begin, d_end, r);
            if (iter == d_end) 
            {
                util::FailMessage::create()
                    .file(d_file)
                    .line(d_line)
                    .fact("Expected collection to contain", r)
                    .fact("Got", d_begin, d_end);
            }
            return *this;
        }

        IntegralSubject<std::size_t> hasSizeThat() 
        {
            size_t sz = std::distance(d_begin, d_end);
            return IntegralSubject<std::size_t>(d_failOnError, d_file, d_line, sz);
        }
    };

}}}//qor::assert::subject