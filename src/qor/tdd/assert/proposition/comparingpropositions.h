// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#ifndef QOR_PP_H_SRC_QOR_TDD_ASSERT_PROPOSITION_COMPARINGPROPOSITIONS
#define QOR_PP_H_SRC_QOR_TDD_ASSERT_PROPOSITION_COMPARINGPROPOSITIONS

#include <memory.h>
#include "../subject/base.h"
#include "../util/failmessage.h"

namespace qor{ namespace assert{ namespace proposition{

    template <typename T, typename U>
    class ComparisonPropositions : public virtual subject::Base 
    {
    protected:
        virtual const U* getValue() const = 0;

    public:
        T& isLessThan(const U& other) 
        {
            if (*getValue() >= other) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("value is not finite", other)
                    .fact("Got", getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isAtMost(const U& other) 
        {
            if (!(*getValue() <= other)) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("less than or equal to", other)
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isGreaterThan(const U& other) 
        {
            if (!(*getValue() > other)) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("greater than", other)
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isAtLeast(const U& other) 
        {
            if (!(*getValue() >= other)) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("greater than or equal to", other)
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }
    };

}}}//qor::assert::proposition

#endif//QOR_PP_H_SRC_QOR_TDD_ASSERT_PROPOSITION_COMPARINGPROPOSITIONS
