// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_CURRENTTHREAD
#define QOR_PP_H_FRAMEWORK_CURRENTTHREAD

#include <thread>

#include "src/platform/compiler/compiler.h"
#include "detail/threadcontext.h"
#include "icurrentthread.h"

namespace qor{

    //All libraries providing an implementation of ICurrentThread 
    //need to export this function so that the linker can find them
    //Here we import it to create a dependency if we aren't the exporter
    bool qor_pp_module_interface(qor_pp_implements_icurrentthread) ImplementsICurrentThread();

    namespace thread
    {
        class qor_pp_module_interface(QOR_THREAD) ThreadPool;       //forward declare ThreadPool class
    }

#ifdef QOR_THREAD   //Anything other than the qor_thread library doesn't need to know about these
    extern qor_pp_thread_local std::optional<std::size_t> pool_index;
    extern qor_pp_thread_local std::optional<void*> parent_pool;
#endif

    //Represent the currently executing thread. Applies whether we have one thread or many.
    class qor_pp_module_interface(QOR_THREAD) CurrentThread final
    {
        friend class qor_pp_module_interface(QOR_THREAD) Thread;
        friend class thread::ThreadPool;

    public:
    
        static const CurrentThread& GetCurrent();
        static const CurrentThread& Get();
        CurrentThread() = default;
        CurrentThread(const CurrentThread & src) = delete;
        CurrentThread& operator=(CurrentThread const& src) = delete;
		~CurrentThread() noexcept = default;

        std::thread::id GetID(void) const;

		template <class _Clock, class _Duration>
		void SleepUntil(const std::chrono::time_point<_Clock, _Duration>& timePoint) const
		{
			std::this_thread::sleep_until(timePoint);
		}

		template <class _Rep, class _Period>
		void SleepFor(const std::chrono::duration<_Rep, _Period>& timePeriod) const
		{
			std::this_thread::sleep_for(timePeriod);
		}

        void Sleep(unsigned long ulMilliseconds) const;
        void _Yield() const;
        detail::ThreadContext& Context() const;
        virtual bool SetPriority(ICurrentThread::Priority priority) const;
        virtual std::optional<ICurrentThread::Priority> GetPriority() const;
        virtual bool SetName(const std::string& name) const;
        virtual std::optional<std::string> GetName() const;
        virtual bool SetAffinity(const std::vector<bool>& affinity) const;
        virtual std::optional<std::vector<bool>> GetAffinity() const;
        std::optional<void*> GetPool() const noexcept;
        std::optional<std::size_t> GetPoolIndex() const noexcept;

    private:

        static void Init();
        static void Destroy();
        static CurrentThread& GetMutableCurrent();

        void SetPool(std::optional<void*> pool);
        void SetIndex(std::optional<std::size_t> index);

        mutable detail::ThreadContext m_Context;
    };

}//qor

#endif//QOR_PP_H_FRAMEWORK_CURRENTTHREAD
