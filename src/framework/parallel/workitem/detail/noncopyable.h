// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PARALLEL_WORKITEM_DETAIL_NONCOPYABLE
#define QOR_PP_H_PARALLEL_WORKITEM_DETAIL_NONCOPYABLE

namespace qor{ namespace par{ namespace detail{

    struct non_copyable
    {
        non_copyable() noexcept = default;
        ~non_copyable() noexcept = default;
        non_copyable(non_copyable const &) = delete;
        non_copyable & operator = (non_copyable const &) = delete;
        non_copyable(non_copyable &&) = default;
        non_copyable & operator = (non_copyable &&) = default;
    };

}}}//qor::par::detail

#endif//QOR_PP_H_PARALLEL_WORKITEM_DETAIL_NONCOPYABLE