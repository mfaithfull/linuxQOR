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

#ifdef QOR_CURRENT   //Anything other than the qor_current library doesn't need to know about these
    extern qor_pp_thread_local std::optional<std::size_t> pool_index;
    extern qor_pp_thread_local std::optional<void*> parent_pool;
#endif

    //Represent the currently executing thread. Applies whether we have one thread or many.
    class CurrentThread final
    {
        friend class qor_pp_module_interface(QOR_THREAD) Thread;
        friend class qor_pp_module_interface(QOR_THREAD) thread::ThreadPool;

    public:
    
        qor_pp_module_interface(QOR_CURRENT) static const CurrentThread& GetCurrent();
        qor_pp_module_interface(QOR_CURRENT) static const CurrentThread& Get();
        qor_pp_module_interface(QOR_CURRENT) CurrentThread();
        qor_pp_module_interface(QOR_CURRENT) CurrentThread(const CurrentThread & src) = delete;
        qor_pp_module_interface(QOR_CURRENT) CurrentThread& operator=(CurrentThread const& src) = delete;
		qor_pp_module_interface(QOR_CURRENT) ~CurrentThread() noexcept;

        qor_pp_module_interface(QOR_CURRENT) std::thread::id GetID(void) const;

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

        qor_pp_module_interface(QOR_CURRENT) void Sleep(unsigned long ulMilliseconds) const;
        qor_pp_module_interface(QOR_CURRENT) void _Yield() const;
        qor_pp_module_interface(QOR_CURRENT) detail::ThreadContext& Context() const;
        qor_pp_module_interface(QOR_CURRENT) virtual bool SetPriority(ICurrentThread::Priority priority) const;
        qor_pp_module_interface(QOR_CURRENT) virtual std::optional<ICurrentThread::Priority> GetPriority() const;
        qor_pp_module_interface(QOR_CURRENT) virtual bool SetName(const std::string& name) const;
        qor_pp_module_interface(QOR_CURRENT) virtual std::optional<std::string> GetName() const;
        qor_pp_module_interface(QOR_CURRENT) virtual bool SetAffinity(const std::vector<bool>& affinity) const;
        qor_pp_module_interface(QOR_CURRENT) virtual std::optional<std::vector<bool>> GetAffinity() const;
        qor_pp_module_interface(QOR_CURRENT) std::optional<void*> GetPool() const noexcept;
        qor_pp_module_interface(QOR_CURRENT) std::optional<std::size_t> GetPoolIndex() const noexcept;

    private:

        qor_pp_module_interface(QOR_CURRENT) static void Init();
        qor_pp_module_interface(QOR_CURRENT) static void Destroy();
        qor_pp_module_interface(QOR_CURRENT) static CurrentThread& GetMutableCurrent();

        qor_pp_module_interface(QOR_CURRENT) void SetPool(std::optional<void*> pool);
        qor_pp_module_interface(QOR_CURRENT) void SetIndex(std::optional<std::size_t> index);

        mutable detail::ThreadContext m_Context;
    };

}//qor

#endif//QOR_PP_H_FRAMEWORK_CURRENTTHREAD
