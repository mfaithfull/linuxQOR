// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_THREADPOOL_TYPES
#define QOR_PP_H_FRAMEWORK_THREADPOOL_TYPES

#include <functional>
#include "src/framework/parallel/thread/thread.h"

namespace qor { namespace thread{

    class ThreadPool;                                                   //forward declare ThreadPool class
#ifdef __cpp_lib_move_only_function
    template <typename... S>                                            //The template to use to store functions in the task queue and other places. In C++23 and later we use `std::move_only_function`.
    using function_t = std::move_only_function<S...>;
#else
    template <typename... S>                                            //The template to use to store functions in the task queue and other places. In C++17 we use `std::function`.
    using function_t = std::function<S...>;
#endif
    using task_t = function_t<void()>;                                  //The type of tasks in the task queue.
    using thread_t = qor::Thread;                                       //The type of threads to use.

}}//qor::thread    

#endif//QOR_PP_H_FRAMEWORK_THREADPOOL_TYPES
