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

#include "src/configuration/configuration.h"
#include "src/framework/thread/currentthread.h"
#include "src/qor/reference/newref.h"

namespace qor{ namespace framework{

    qor_pp_thread_local CurrentThread* t_pCurrentThread = nullptr;
    template<> qor_pp_thread_local bool qor::detail::ThreadInstanceHolder<framework::ICurrentThread>::bInitialised = false;
    template<> thread_local typename ref_of<framework::ICurrentThread>::type qor::detail::ThreadInstanceHolder<framework::ICurrentThread>::theRef = qor::ThreadSingletonInstancer::template Instance<framework::ICurrentThread>(1);

	const CurrentThread& CurrentThread::GetCurrent()
	{
		if (t_pCurrentThread == nullptr)
		{
			t_pCurrentThread = new CurrentThread;
		}
        return *t_pCurrentThread;
	}

	CurrentThread& CurrentThread::GetMutableCurrent()
	{
		if (t_pCurrentThread == nullptr)
		{
			t_pCurrentThread = new CurrentThread;
		}
        return *t_pCurrentThread;
	}

    void SetCurrent(CurrentThread* pThread)
    {
        t_pCurrentThread = pThread;
    }

    std::thread::id CurrentThread::GetID() const
    {
        return std::this_thread::get_id();
    }

	void CurrentThread::Sleep(unsigned long ulMilliseconds) const
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ulMilliseconds));
	}

    void CurrentThread::Yield() const
	{		
		std::this_thread::yield();
	}

    ThreadContext& CurrentThread::Context() const
    {
        return m_Context;
    }

    bool CurrentThread::SetPriority(ICurrentThread::Priority priority) const
    {
        return new_ref<ICurrentThread>()().SetPriority(priority);
    }

    std::optional<ICurrentThread::Priority> CurrentThread::GetPriority() const
    {
        return new_ref<ICurrentThread>()().GetPriority();
    }

    bool CurrentThread::SetName(const std::string& name) const
    {
        return new_ref<ICurrentThread>()().SetName(name);
    }

    std::optional<std::string> CurrentThread::GetName() const
    {
        return new_ref<ICurrentThread>()().GetName();
    }

    bool CurrentThread::SetAffinity(const std::vector<bool>& affinity) const
    {
        return new_ref<ICurrentThread>()().SetAffinity(affinity);
    }

    std::optional<std::vector<bool>> CurrentThread::GetAffinity() const
    {
        return new_ref<ICurrentThread>()().GetAffinity();
    }

}}//qor::framework