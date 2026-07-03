// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#pragma once

#include <memory.h>
#include "../subject/base.h"
#include "../util/failmessage.h"

namespace qor{ namespace assert{ namespace proposition{

    template <typename T, typename U>
    class ContainsKeyPropositions : public virtual subject::Base 
    {
    protected:
        virtual const U* getValue() const = 0;

    public:
        template <typename Key>
        T& containsKey(const Key key) 
        {
            auto iter = getValue()->find(key);
            if (iter == getValue()->end()) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("to contain the key ", key)
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }

        template <typename Key>
        T& doesNotContainKey(const Key key) 
        {
            auto iter = getValue()->find(key);
            if (iter != getValue()->end()) 
            {
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("to not contain the key ", key)
                    .fact("Got", *getValue());
            }
            return *dynamic_cast<T*>(this);
        }
    };

}}}//qor::assert::proposition
