// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "threadpool.h"

namespace qor{ namespace thread{

    ThreadPool::ThreadPool()                                                                                    //Construct a new thread pool. The number of threads will be derived from total number of hardware threads available, as reported by the implementation. This is usually determined by the number of cores in the CPU. If a core is hyperthreaded, it will count as two threads.
    {
        SetThreadCount(0);
        SetInitFunction([] {});
    }

    ThreadPool::ThreadPool(const std::size_t num_threads)                                              //Construct a new thread pool with the specified number of threads.
    {
        SetThreadCount(num_threads);
        SetInitFunction([] {});
    }

    void ThreadPool::Setup()
    {
        CreateThreads();
    }

    void ThreadPool::Shutdown()
    {
        Wait();
        Purge();
        threads = {};
    }

    ThreadPool::~ThreadPool() noexcept                                                                          //Destruct the thread pool. Waits for all tasks to complete, then destroys all threads. If a cleanup function was set, it will run in each thread right before it is destroyed. Note that if the pool is paused, then any tasks still in the queue will never be executed.
    {            
#ifdef __cpp_exceptions
        try
        {
#endif
            Wait();
#ifdef __cpp_exceptions
        }
        catch (...)
        {
        }
#endif
        while(tasks.size())
        {
            tasks.pop();
        }
    }


    //Get a vector containing the underlying implementation-defined thread handles for each of the pool's threads, as obtained by `std::thread::native_handle()` (or `std::jthread::native_handle()` in C++20 and later).
    //return The native thread handles.     
    std::vector<thread_t::native_handle_type> ThreadPool::GetNativeHandles() const
    {
        std::vector<thread_t::native_handle_type> native_handles(thread_count);
        for (std::size_t i = 0; i < thread_count; ++i)
            native_handles[i] = threads[i].stdThread().native_handle();
        return native_handles;
    }

    std::size_t ThreadPool::GetCountOfTasksQueued() const
    {
        const std::scoped_lock tasks_lock(tasks_mutex);
        return tasks.size();
    }

    //Get the number of tasks currently being executed by the threads.
    std::size_t ThreadPool::GetCountOfTasksRunning() const
    {
        const std::scoped_lock tasks_lock(tasks_mutex);
        return tasks_running;
    }

    //Get the total number of unfinished tasks: either still waiting in the queue, or running in a thread. Note that `GetTotalCountOfTasks() == GetCountOfTasksQueued() + GetCountOfTasksRunning()`.
    //return The total number of tasks.
    std::size_t ThreadPool::GetTotalCountOfTasks() const
    {
        const std::scoped_lock tasks_lock(tasks_mutex);
        return tasks_running + tasks.size();
    }

    std::size_t ThreadPool::GetThreadCount() const noexcept
    {
        return thread_count;
    }

    std::vector<thread_t::id> ThreadPool::GetThreadIds() const
    {
        std::vector<thread_t::id> thread_ids(thread_count);
        for (std::size_t i = 0; i < thread_count; ++i)
        {
            thread_ids[i] = threads[i].stdThread().get_id();
        }
        return thread_ids;
    }

    bool ThreadPool::IsPaused() const
    {
        const std::scoped_lock tasks_lock(tasks_mutex);
        return paused;
    }

    void ThreadPool::Pause()
    {
        const std::scoped_lock tasks_lock(tasks_mutex);
        paused = true;
    }

    void ThreadPool::Purge()
    {
        const std::scoped_lock tasks_lock(tasks_mutex);
        tasks = {};
    }

    void ThreadPool::Reset()
    {
        Reset(0, [](std::size_t) {});
    }

    void ThreadPool::Reset(const std::size_t num_threads)
    {
        Reset(num_threads, [](std::size_t) {});
    }

    void ThreadPool::Unpause()
    {
        {
            const std::scoped_lock tasks_lock(tasks_mutex);
            paused = false;
        }
        task_available_cv.notify_all();
    }

    void ThreadPool::Wait()
    {
#ifdef __cpp_exceptions
        if (wait_deadlock_checks_enabled)
        {
            if (CurrentThread::GetCurrent().GetPool() == this)
                throw wait_deadlock();
        }
#endif
        std::unique_lock tasks_lock(tasks_mutex);
        waiting = true;
        tasks_done_cv.wait(tasks_lock,
            [this]
            {
                return (tasks_running == 0) && (paused || tasks.empty());
            });
        waiting = false;
    }

    auto ThreadPool::Schedule() 
    {
        struct Awaiter : std::suspend_always 
        {
            ThreadPool &tpool;
            Awaiter(ThreadPool &pool) : tpool{pool} {}
            void await_suspend(std::coroutine_handle<> handle) 
            {
                tpool.PostTask([handle, this]() { handle.resume(); });
            }
        };
        return Awaiter{*this};
    }

    void ThreadPool::SetThreadCount(const std::size_t num_threads)
    {
        thread_count = DetermineThreadCount(num_threads);
    }

    void ThreadPool::CreateThreads()
    {
        threads = std::make_unique<thread_t[]>(thread_count);
        {
            const std::scoped_lock tasks_lock(tasks_mutex);
            tasks_running = thread_count;
        }

        for (std::size_t i = 0; i < thread_count; ++i)
        {
            threads[i] = thread_t(
                [this, i]
                (const std::stop_token& stop_token)
                {               
                    CurrentThread::Init();                    
                    Worker(stop_token, i);
                    CurrentThread::Destroy();
                }
            );
        }
    }

    std::size_t ThreadPool::DetermineThreadCount(const std::size_t num_threads) noexcept
    {
        if (num_threads > 0)
            return num_threads;
        if (thread_t::std_thread_t::hardware_concurrency() > 2)
            return 1 + ( thread_t::std_thread_t::hardware_concurrency() - 2 );
        if (thread_t::std_thread_t::hardware_concurrency() > 0)
            return 1 + ( thread_t::std_thread_t::hardware_concurrency() - 1 );
        return 1;
    }

    task_t ThreadPool::PopTask()
    {
        task_t task;
        task = std::move(const_cast<pr_task&>(tasks.top()).task);
        tasks.pop();
        return task;
    }

    void ThreadPool::Worker(const std::stop_token &stop_token, const std::size_t idx)
    {
        CurrentThread::GetMutableCurrent().SetPool(this);
        CurrentThread::GetMutableCurrent().SetIndex(idx);
        init_func(idx);
        while (true)
        {
            std::unique_lock tasks_lock(tasks_mutex);
            --tasks_running;
            if (waiting && (tasks_running == 0) && ( paused || tasks.empty()))
            {
                tasks_done_cv.notify_all();
            }
            //tasks_lock is unlocked by the wait here
            task_available_cv.wait(tasks_lock , stop_token,
                [this]
                {
                    return !( paused || tasks.empty());
                });
            //tasks_lock is relocked by the wait before here
            if (stop_token.stop_requested())
            {
                break;
            }
            {
                task_t task = PopTask(); // NOLINT(misc-const-correctness) In C++23 this cannot be const since `std::move_only_function::operator()` is not a const member function.
                ++tasks_running;
                tasks_lock.unlock();
#ifdef __cpp_exceptions
                try
                {
#endif
                    task();
#ifdef __cpp_exceptions
                }
                catch (...)
                {
                }
#endif
            }
        }
        cleanup_func(idx);
        CurrentThread::GetMutableCurrent().SetIndex(std::nullopt);
        CurrentThread::GetMutableCurrent().SetPool(std::nullopt);
    }

}}//qor::thread
