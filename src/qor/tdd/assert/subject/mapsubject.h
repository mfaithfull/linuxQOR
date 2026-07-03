// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license
#pragma once

#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "../proposition/containskeypropositions.h"
#include "../proposition/hassizepropositions.h"
#include "../proposition/isemptypropositions.h"
#include "../proposition/isequaltopropositions.h"
#include "base.h"

namespace qor{ namespace assert{ namespace subject{

    template <typename T>
    class MapSubject : public virtual Base,
                    public proposition::ContainsKeyPropositions<MapSubject<T>, T>,
                    public proposition::HasSizePropositions<MapSubject<T>, T>,
                    public proposition::IsEmptyPropositions<MapSubject<T>, T>,
                    public proposition::IsEqualToPropositions<MapSubject<T>, T> 
    {
        const T d_value;

        protected:
        const T* getValue() const override { return &d_value; }

        public:
        MapSubject(const bool failOnError, const char* file, int line, const T& v)
            : Base(failOnError, file, line), d_value(v) {}

        template <typename Key, typename Value>
        auto& containsEntry(Key key, Value value) {
            auto iter = getValue()->find(key);
            if (iter == getValue()->end() || iter->second != value) 
            {
                std::stringstream ss;
                ss << "(" << key << ", " << value << ")";
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("Expected the map contains the entry: ", ss.str())
                    .fact("Collection did not contain the entry");
            }
            return *this;
        }

        template <typename Key, typename Value>
        auto& doesNotContainEntry(Key key, Value value) 
        {
            auto iter = getValue()->find(key);
            if (iter != getValue()->end() && iter->second == value) 
            {
                std::stringstream ss;
                ss << "(" << key << ", " << value << ")";
                util::FailMessage::create()
                    .file(getFile())
                    .line(getLine())
                    .fact("Expected the map does not contains the entry: ", ss.str())
                    .fact("The collection does contain the entry");
            }
            return *this;
        }
    };

}}}//qor::assert::subject