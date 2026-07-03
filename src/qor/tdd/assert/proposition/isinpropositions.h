// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#ifndef QOR_PP_H_ASSERT_ISINPROPOSITION
#define QOR_PP_H_ASSERT_ISINPROPOSITION

#include <memory.h>

#include "../subject/base.h"
#include "../util/failmessage.h"

namespace qor{ namespace assert{ namespace proposition{

    template <typename T, typename U>
    class IsInPropositions : public virtual subject::Base 
    {
    private:
        template <typename List, typename I>
        bool checkIsInRange(List& r, const I& value) 
        {
            for (const auto& e : r) 
            {
                if (value == e) return true;
            }
            return false;
        }

        template <typename I>
        static bool checkIsInRange(std::initializer_list<I> r, const I& value) 
        {
            for (const auto& e : r) 
            {
                if (value == e) return true;
            }
            return false;
        }

    protected:
        virtual const U* getValue() const = 0;

    public:
        T& isIn(std::initializer_list<U> r) 
        {
            if (!checkIsInRange(r, *getValue())) 
            {
                util::FailMessage::create()
                .file(getFile())
                .line(getLine())
                .fact("is in range", r.begin(), r.end())
                .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        T& isNotIn(std::initializer_list<U> r) 
        {
            if (checkIsInRange(r, *getValue())) 
            {
                util::FailMessage::create()
                .file(getFile())
                .line(getLine())
                .fact("is not in range", r.begin(), r.end())
                .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        template <typename List>
        T& isIn(List r) 
        {
            if (!checkIsInRange(r, *getValue())) 
            {
                util::FailMessage::create()
                .file(getFile())
                .line(getLine())
                .fact("is in range", r.begin(), r.end())
                .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        template< template <typename...> typename List >
        T& isIn(List<U> r) 
        {
            if (!checkIsInRange(r, *getValue())) 
            {
                util::FailMessage::create()
                .file(getFile())
                .line(getLine())
                .fact("is in range", r.begin(), r.end())
                .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        template< template <typename...> typename List >
        T& isNotIn(List<U> r) 
        {
            if (checkIsInRange(r, *getValue())) 
            {
                util::FailMessage::create()
                .file(getFile())
                .line(getLine())
                .fact("is not in range", r.begin(), r.end())
                .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }
    };

}}}//qor::assert::proposition

#endif//QOR_PP_H_ASSERT_ISINPROPOSITION
