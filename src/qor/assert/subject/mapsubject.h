// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

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
                   public proposition::IsEqualToPropositions<MapSubject<T>, T> {
  const T d_value;

 protected:
  const T* getValue() const override { return &d_value; }

 public:
  MapSubject(const bool failOnError, const char* file, int line, const T& v)
      : Base(failOnError, file, line), d_value(v) {}

  template <typename Key, typename Value>
  auto& containsEntry(Key key, Value value) {
    auto iter = getValue()->find(key);
    if (iter == getValue()->end() || iter->second != value) {
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
  auto& doesNotContainEntry(Key key, Value value) {
    auto iter = getValue()->find(key);
    if (iter != getValue()->end() && iter->second == value) {
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