// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTIONCONTEXT
#define QOR_PP_H_FUNCTIONCONTEXT

#include <source_location>

#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/tdd/profiling/profiling.h"
#include "src/qor/tdd/profiling/iprofilereceiver.h"
#include "src/qor/flyers/interception/ifunctioncontext.h"
#include "src/qor/flyers/interception/callcontext.h"

//A class for tracking function context
//Usage: Only ever create these on the stack at the beginning of a function

//Use qor_pp_fcontext in free functions
#define qor_pp_fcontext qor::FunctionContext _FContext_(std::source_location::current(), ThisModule().Name(), qor_pp_profile_enabled, qor_pp_coverage_enabled)

//Use qor_pp_ofcontext in member functions
#define qor_pp_ofcontext qor::FunctionContext _FContext_(std::source_location::current(), ThisModule().Name(), qor_pp_profile_enabled, qor_pp_coverage_enabled, qor::AnyObject(this))

namespace qor
{
	class qor_pp_module_interface(QOR_INTERCEPTION) FunctionContext : public IProfileReceiver, public IFunctionContext
	{
	public:

		FunctionContext();
		FunctionContext(const FunctionContext&) = delete;
		FunctionContext& operator = (const FunctionContext&) = delete;
		FunctionContext(const std::source_location& location, const char* module, bool profile, bool coverage, AnyObject objContext = AnyObject::EmptyObject());
		virtual ~FunctionContext();
		virtual unsigned int Lock() noexcept override;
		virtual unsigned int Unlock() noexcept override;		
		virtual bool Locked() const noexcept override;
		virtual ICallContext* GetCallContext() noexcept override;
		virtual IFunctionContext* GetParent() const override;
		virtual void SetParent(IFunctionContext* parent) override;
		virtual unsigned int TraceDepth() const noexcept override;
		const char* File() const noexcept override;
		const char* Name() const noexcept override;;
		unsigned int Line() const noexcept override;
		unsigned int Column() const noexcept override;
		AnyObject TypedAny() const override;
		virtual const char* Module() const override;
        virtual void Profile(const std::chrono::duration<int64_t, std::micro>, IFunctionContext* fContext);
		bool Trace(bool newTrace);

	protected:

		bool m_traceCalls{false};
		unsigned int m_locked{0};
		unsigned int m_traceDepth{1};
		std::source_location m_location{};
		const char* m_module{nullptr};
		IFunctionContext* m_Parent{nullptr};
		AnyObject m_ObjContext;
		CallContext m_CallContext;
		qor_pp_profiling_object m_Profiler;

		virtual void Init();

	private:

		void* operator new(size_t, void*) noexcept = delete;	//prevent accidental heap allocation
	};

}//qor

#endif//QOR_PP_H_FUNCTIONCONTEXT
