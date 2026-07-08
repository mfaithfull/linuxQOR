// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_THREADPOOL_CONCEPTS
#define QOR_PP_H_FRAMEWORK_THREADPOOL_CONCEPTS

namespace qor { namespace thread{

#ifdef __cpp_concepts      // In C++20 and later we can use concepts. In C++17 we instead use SFINAE ("Substitution Failure Is Not An Error") with `std::enable_if_t`.
    template <typename F> concept init_func_c = std::invocable<F> || std::invocable<F, std::size_t>;
    #define qor_pp_threadpool_init_func_concept(F) init_func_c F
#else
    #define qor_pp_threadpool_init_func_concept(F) typename F, typename = std::enable_if_t<std::is_invocable_v<F> || std::is_invocable_v<F, std::size_t>>
#endif

}}//qor::thread

#endif//QOR_PP_H_FRAMEWORK_THREADPOOL_CONCEPTS
