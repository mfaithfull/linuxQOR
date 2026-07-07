// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "functioncontext.h"
#include "src/qor/essentials/current/currentthread.h"

namespace qor {

	FunctionContext::FunctionContext(const char* funcName, const char* file, unsigned int line, const char* module, bool profile, bool /*coverage*/, AnyObject objContext) :
		 m_traceCalls(false)
		, m_locked(0)
		, m_traceDepth(1)
		, m_funcName(funcName)
		, m_file(file)
		, m_module(module)
		, m_Parent(nullptr)
		, m_ObjContext(objContext)
		, m_line(line)
		, m_CallContext()
		, m_Profiler(this, profile)
	{
		Init();
	}

	FunctionContext::FunctionContext() :
		m_traceCalls(false)
		, m_locked(0)
		, m_traceDepth(1)
		, m_funcName(nullptr)
		, m_file(nullptr)
		, m_module(nullptr)
		, m_Parent(nullptr)
		, m_line(0)
		, m_CallContext()
		, m_Profiler(this, false)
	{
		Init();
	}

	bool FunctionContext::Locked() const
	{
		return m_locked > 0;
	}

	unsigned int FunctionContext::Lock()
	{
		return ++m_locked;
	}

	unsigned int FunctionContext::Unlock()
	{
		return --m_locked;
	}

	bool FunctionContext::Trace(bool newTrace)
	{
		bool bTrace = m_traceCalls;
		m_traceCalls = newTrace;
		return bTrace;
	}

	ICallContext* FunctionContext::GetCallContext(void)
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

	const char* FunctionContext::File() const
	{
		return m_file;
	}

	const char* FunctionContext::Module() const
	{
		return m_module;
	}

	unsigned int FunctionContext::Line() const
	{
		return m_line;
	}

	IFunctionContext* FunctionContext::GetParent() const
	{
		return m_Parent;
	}

	void FunctionContext::SetParent(IFunctionContext* pParent)
	{
		m_Parent = pParent;
	}

	unsigned int FunctionContext::TraceDepth()
	{
		return m_traceDepth;
	}

	FunctionContext::~FunctionContext()
	{
		Lock();
		if (m_Parent)
		{
			if (!m_Parent->Locked() && m_Parent->GetCallContext())
			{
				m_Parent->GetCallContext()->CallCompleted();//Tell call context we have completed sub function body
			}
		}

        CurrentThread::GetCurrent().Context().UnregisterFunctionContext(this, m_Parent);

        Unlock();
	}

	const char* FunctionContext::Name() const
	{
		return m_funcName;
	}

	AnyObject FunctionContext::TypedAny() const
	{
		return m_ObjContext;
	}

	void FunctionContext::Profile(const std::chrono::duration<int64_t, std::milli> /*duration*/, IFunctionContext* /*ignore*/)
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
//Review 2027/07/07