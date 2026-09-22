// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PARALLEL_WORKITEM_DETAIL_NONMOVABLE
#define QOR_PP_H_PARALLEL_WORKITEM_DETAIL_NONMOVABLE

namespace qor{ namespace par{ namespace detail{

    struct non_movable
    {
        non_movable() noexcept = default;
        ~non_movable() noexcept = default;
        non_movable(non_movable &&) = delete;
        non_movable & operator = (non_movable &&) = delete;
        non_movable(non_movable const &) = default;
        non_movable & operator = (non_movable const &) = default;
    };

}}}//qor::par::detail
#endif//QOR_PP_H_PARALLEL_WORKITEM_DETAIL_NONMOVABLE
