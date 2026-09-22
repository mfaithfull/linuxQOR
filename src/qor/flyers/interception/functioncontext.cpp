// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "functioncontext.h"
#include "src/qor/essentials/current/currentthread.h"

namespace qor {

	FunctionContext::FunctionContext(const std::source_location& location, const char* module, bool profile, bool /*coverage*/, AnyObject objContext) :
		 m_traceCalls(false)
		, m_locked(0)
		, m_traceDepth(1)
		, m_location(location)
		, m_module(module)
		, m_Parent(nullptr)
		, m_ObjContext(objContext)
		, m_CallContext()
		, m_Profiler(this, profile)
	{
		Init();
	}

	FunctionContext::FunctionContext() :
		m_traceCalls(false)
		, m_locked(0)
		, m_traceDepth(1)
		, m_module(nullptr)
		, m_Parent(nullptr)
		, m_CallContext()
		, m_Profiler(this, false)
	{
		Init();
	}

	bool FunctionContext::Locked() const noexcept
	{
		return m_locked > 0;
	}

	unsigned int FunctionContext::Lock() noexcept
	{
		return ++m_locked;
	}

	unsigned int FunctionContext::Unlock() noexcept
	{
		return --m_locked;
	}

	bool FunctionContext::Trace(bool newTrace)
	{
		bool bTrace = m_traceCalls;
		m_traceCalls = newTrace;
		return bTrace;
	}

	ICallContext* FunctionContext::GetCallContext(void) noexcept
	{
		return &m_CallContext;
	}

	void FunctionContext::Init()
	{
		Lock();

        m_Parent = CurrentThread::GetCurrent().Context().RegisterFunctionContext(this);

		if (m_Parent)
		{
			m_traceDepth = m_Parent->TraceDepth() + 1;
			if (!m_Parent->Locked())
			{
				if (m_Parent->GetCallContext())
				{
					m_Parent->GetCallContext()->CallMade(this);//Tell calling context we have reached called function body
				}
			}
		}
		Unlock();
	}

	const char* FunctionContext::File() const noexcept
	{
		return m_location.file_name();
	}

	const char* FunctionContext::Module() const
	{
		return m_module;
	}

	unsigned int FunctionContext::Line() const noexcept
	{
		return m_location.line();
	}

	unsigned int FunctionContext::Column() const noexcept
	{
		return m_location.column();
	}

	IFunctionContext* FunctionContext::GetParent() const
	{
		return m_Parent;
	}

	void FunctionContext::SetParent(IFunctionContext* pParent)
	{
		m_Parent = pParent;
	}

	unsigned int FunctionContext::TraceDepth() const noexcept
	{
		return m_traceDepth;
	}

	FunctionContext::~FunctionContext()
	{
		FunctionContext::Lock();
		if (m_Parent)
		{
			if (!m_Parent->Locked() && m_Parent->GetCallContext())
			{
				m_Parent->GetCallContext()->CallCompleted();//Tell call context we have completed sub function body
			}
		}

        CurrentThread::GetCurrent().Context().UnregisterFunctionContext(this, m_Parent);

        FunctionContext::Unlock();
	}

	const char* FunctionContext::Name() const noexcept
	{
		return m_location.function_name();
	}

	AnyObject FunctionContext::TypedAny() const
	{
		return m_ObjContext;
	}

	void FunctionContext::Profile(const std::chrono::duration<int64_t, std::micro> /*duration*/, IFunctionContext* /*ignore*/)
	{
		/*
        auto profileHandler = new_ref< ProfileReceiver >();
        if(!profileHandler.IsNull())
        {
            profileHandler->Profile(duration, this);
        }
		*/
	}

}//qor
