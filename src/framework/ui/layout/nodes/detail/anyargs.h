// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_DETAIL_ANYARGS
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_DETAIL_ANYARGS

#include <deque>
#include <queue>
#include <stack>
#include <vector>

#include "../../node.h"

namespace qor{ namespace ui{ namespace layout { namespace detail {

    inline void Merge(Elements& container, Element element) 
    {
        container.push_back(std::move(element));
    }

    // Turn a set of arguments into a vector.
    template <class... Args>
    Elements unpack(Args... args) 
    {
        std::vector<Element> vec;
        (Merge(vec, std::move(args)), ...);
        return vec;
    }

    // Make |container| able to take any number of arguments.
#define TAKE_ANY_ARGS(container)                               \
  inline Element container(Element child) {                    \
    return container(detail::unpack(std::move(child)));                \
  }                                                            \
                                                               \
  template <class... Args>                                     \
  inline Element container(Args... children) {                 \
    return container(detail::unpack(std::forward<Args>(children)...)); \
  }                                                            \
                                                               \
  template <class Container>                                   \
  inline Element container(Container&& children) {             \
    Elements elements;                                         \
    for (auto& child : children) {                             \
      elements.push_back(std::move(child));                    \
    }                                                          \
    return container(std::move(elements));                     \
  }                                                            \
  template <>                                                  \
  inline Element container(std::stack<Element>&& children) {   \
    Elements elements;                                         \
    while (!children.empty()) {                                \
      elements.push_back(std::move(children.top()));           \
      children.pop();                                          \
    }                                                          \
    return container(std::move(elements));                     \
  }                                                            \
  template <>                                                  \
  inline Element container(std::queue<Element>&& children) {   \
    Elements elements;                                         \
    while (!children.empty()) {                                \
      elements.push_back(std::move(children.front()));         \
      children.pop();                                          \
    }                                                          \
    return container(std::move(elements));                     \
  }                                                            \
  template <>                                                  \
  inline Element container(std::deque<Element>&& children) {   \
    Elements elements;                                         \
    for (auto& child : children) {                             \
      elements.push_back(std::move(child));                    \
    }                                                          \
    return container(std::move(elements));                     \
  }

    //TAKE_ANY_ARGS(vbox)
    //TAKE_ANY_ARGS(hbox)
    //TAKE_ANY_ARGS(dbox)
    //TAKE_ANY_ARGS(hflow)

}}}}//qor::ui::layout::detail

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_DETAIL_ANYARGS

