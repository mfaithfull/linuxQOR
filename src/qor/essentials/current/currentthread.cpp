// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"

namespace qor
{ 
    namespace detail 
    {
        thread_local ThreadInstanceHolder<ICurrentThread> ThreadInstanceHolderCurrentPlatformThread;
        thread_local ThreadInstanceHolder<CurrentThread> ThreadInstanceHolderCurrentThread;
        
        qor_pp_export ThreadInstanceHolder<ICurrentThread>* GetCurrentPlatformThreadHolder()
        {
            return &ThreadInstanceHolderCurrentPlatformThread;
        }

        qor_pp_export ThreadInstanceHolder<CurrentThread>* GetCurrentThreadHolder()
        {
            return &ThreadInstanceHolderCurrentThread;
        }

        template<>
        qor_pp_export ThreadInstanceHolder<ICurrentThread>* theThreadInstanceHolder<ICurrentThread>()
        {
            return GetCurrentPlatformThreadHolder();
        }

        template<>
        qor_pp_export ThreadInstanceHolder<CurrentThread>* theThreadInstanceHolder<CurrentThread>()
        {
            return GetCurrentThreadHolder();
        }

    }//detail

    qor_pp_thread_local std::optional<std::size_t> pool_index = std::nullopt;
    qor_pp_thread_local std::optional<void*> parent_pool = std::nullopt;

    std::optional<void*> CurrentThread::GetPool() const noexcept
    {
        return parent_pool;
    }

    std::optional<std::size_t> CurrentThread::GetPoolIndex() const noexcept
    {
        return pool_index;
    }

    CurrentThread::CurrentThread(){ }
    CurrentThread::~CurrentThread(){ }
    
    void CurrentThread::SetPool(std::optional<void*> pool)
    {
        parent_pool = pool;
    }

    void CurrentThread::SetIndex(std::optional<std::size_t> index)
    {
        pool_index = index;
    }

	const CurrentThread& CurrentThread::GetCurrent()
	{
        return detail::GetCurrentThreadHolder()->Instance().operator const qor::CurrentThread &();
	}

	const CurrentThread& CurrentThread::Get()
	{
        return detail::GetCurrentThreadHolder()->Instance().operator const qor::CurrentThread &();
	}

    void CurrentThread::Init(){ }

    void CurrentThread::Destroy(){ }

	CurrentThread& CurrentThread::GetMutableCurrent()
	{
        return detail::GetCurrentThreadHolder()->Instance()()();
	}

    std::thread::id CurrentThread::GetID() const
    {
        return std::this_thread::get_id();
    }

	void CurrentThread::Sleep(unsigned long ulMilliseconds) const
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ulMilliseconds));
	}

    void CurrentThread::_Yield() const
	{		
		std::this_thread::yield();
	}

    detail::ThreadContext& CurrentThread::Context() const
    {
        return m_Context;
    }

    bool CurrentThread::SetPriority(ICurrentThread::Priority priority) const
    {
        return new_ref<ICurrentThread>()->SetPriority(priority);
    }

    std::optional<ICurrentThread::Priority> CurrentThread::GetPriority() const
    {
        return new_ref<ICurrentThread>()->GetPriority();
    }

    bool CurrentThread::SetName(const std::string& name) const
    {
        return new_ref<ICurrentThread>()->SetName(name);
    }

    std::optional<std::string> CurrentThread::GetName() const
    {
        return new_ref<ICurrentThread>()->GetName();
    }

    bool CurrentThread::SetAffinity(const std::vector<bool>& affinity) const
    {
        return new_ref<ICurrentThread>()->SetAffinity(affinity);
    }

    std::optional<std::vector<bool>> CurrentThread::GetAffinity() const
    {
        return new_ref<ICurrentThread>()->GetAffinity();
    }

}//qor