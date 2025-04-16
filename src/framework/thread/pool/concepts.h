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

#ifndef QOR_PP_H_FRAMEWORK_THREADPOOL_CONCEPTS
#define QOR_PP_H_FRAMEWORK_THREADPOOL_CONCEPTS

namespace qor { namespace framework{

#ifdef __cpp_concepts      // In C++20 and later we can use concepts. In C++17 we instead use SFINAE ("Substitution Failure Is Not An Error") with `std::enable_if_t`.
    #define qor_pp_threadpool_if_pause_enabled template <bool P = pause_enabled> requires(P)
    template <typename F> concept init_func_c = std::invocable<F> || std::invocable<F, std::size_t>;
    #define qor_pp_threadpool_init_func_concept(F) init_func_c F
#else
    #define qor_pp_threadpool_if_pause_enabled template <bool P = pause_enabled, typename = std::enable_if_t<P>>
    #define qor_pp_threadpool_init_func_concept(F) typename F, typename = std::enable_if_t<std::is_invocable_v<F> || std::is_invocable_v<F, std::size_t>>
#endif

}}//qor::framework

#endif//QOR_PP_H_FRAMEWORK_THREADPOOL_CONCEPTS
