// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#pragma once

#include <limits>
#include <memory>

#include "../subject/base.h"
#include "../util/failmessage.h"

namespace qor{ namespace assert{ namespace proposition{

    template <typename T, typename U>
    class FloatingPropositions : public virtual subject::Base 
    {
        protected:
        virtual const U* getValue() const = 0;
        virtual const U* getTolerance() const = 0;

        public:
        T& isEqualTo(const U& other) 
        {
            double relativeDifference = (*getValue() - other) / other;
            if (std::abs(relativeDifference) >= *getTolerance()) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("value is equal to", other)
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isNotEqualTo(const U& other) 
        {
            double relativeDifference = (*getValue() - other) / other;
            if (std::abs(relativeDifference) < *getTolerance()) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("value is notequal to", other)
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isNan() 
        {
            if (!std::isnan(*getValue())) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("value is not a number")
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isNotNan() 
        {
            if (std::isnan(*getValue())) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("value is not the special not a number value")
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isFinite() 
        {
            if (!std::isfinite(*getValue())) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("value is finite")
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isNotFinite() 
        {
            if (std::isfinite(*getValue())) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("value is not finite")
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isPositiveInfinity() 
        {
            if ((*getValue()) != std::numeric_limits<U>::infinity()) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("value is not finite")
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isNegativeInfinity() 
        {
            if ((*getValue()) != -std::numeric_limits<U>::infinity()) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("value is not finite")
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isZero() 
        {
            if ((*getValue()) != 0.0) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("value is zero")
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isNotZero() 
        {
            if ((*getValue()) == 0.0) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("value is not zero")
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }
    };

}}}//qor::assert::proposition

