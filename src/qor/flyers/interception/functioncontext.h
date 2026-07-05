// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTIONCONTEXT
#define QOR_PP_H_FUNCTIONCONTEXT

#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/tdd/profiling/profiling.h"
#include "src/qor/tdd/profiling/iprofilereceiver.h"
#include "src/qor/flyers/interception/ifunctioncontext.h"
#include "src/qor/flyers/interception/callcontext.h"

//A class for tracking function context 
//Usage: Only ever create these on the stack at the beginning of a function

//Use qor_pp_fcontext in free functions
#define qor_pp_fcontext qor::FunctionContext _FContext_(qor_pp_funcsig, __FILE__, __LINE__, ThisModule().Name(), qor_pp_profile_enabled, qor_pp_coverage_enabled)

//Use qor_pp_ofcontext in member functions
#define qor_pp_ofcontext qor::FunctionContext _FContext_(qor_pp_funcsig, __FILE__, __LINE__, ThisModule().Name(), qor_pp_profile_enabled, qor_pp_coverage_enabled, qor::AnyObject(this))

namespace qor
{
	class qor_pp_module_interface(QOR_INTERCEPTION) FunctionContext : public IProfileReceiver, public IFunctionContext
	{
	public:

		FunctionContext();
		FunctionContext(const FunctionContext&) = delete;
		FunctionContext& operator = (const FunctionContext&) = delete;
		FunctionContext(const char* funcName, const char* file, unsigned int line, const char* module, bool profile, bool coverage, AnyObject objContext = AnyObject::EmptyObject());
		virtual ~FunctionContext();
		bool Locked() const;
		virtual unsigned int Lock();
		virtual unsigned int Unlock();
		bool Trace(bool newTrace);
		virtual ICallContext* GetCallContext();
		const char* File() const;
		const char* Name() const;
		unsigned int Line() const;
		virtual IFunctionContext* GetParent() const;
		virtual void SetParent(IFunctionContext* parent);
		virtual unsigned int TraceDepth();
		AnyObject TypedAny() const;
		virtual const char* Module() const;
        virtual void Profile(const std::chrono::duration<int64_t, std::milli>, IFunctionContext* fContext);

	protected:
		
		bool m_traceCalls{false};
		unsigned int m_locked{0};
		unsigned int m_traceDepth{1};
		const char* m_funcName{nullptr};
		const char* m_file{nullptr};
		const char* m_module{nullptr};
		IFunctionContext* m_Parent{nullptr};
		AnyObject m_ObjContext;
		unsigned int m_line{0};
		CallContext m_CallContext;		
		qor_pp_profiling_object m_Profiler;	

		virtual void Init();

	private:
		
		void* operator new(size_t, void*) noexcept = delete;	//prevent accidental heap allocation
	};

}//qor

#endif//QOR_PP_H_FUNCTIONCONTEXT
