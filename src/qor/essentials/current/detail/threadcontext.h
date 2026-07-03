// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_THREADCONTEXT
#define QOR_PP_H_FRAMEWORK_THREADCONTEXT

#include <thread>
#include <vector>

#include "src/platform/compiler/compiler.h"
#include "src/qor/flyers/interception/ifunctioncontext.h"
#include "src/qor/essentials/current/detail/flyermap.h"

namespace qor{ namespace detail{

	//Additional state for QOR threads beyond what std library threads provide
	//Per thread state is only created if it's used via the CurrentThread thread-singleton
    class qor_pp_module_interface(QOR_THREAD) ThreadContext final
    {
    public:

        ThreadContext();
		ThreadContext(const ThreadContext & src) = delete;
		ThreadContext& operator=(ThreadContext const& src) = delete;
		~ThreadContext() = default;

		virtual IFunctionContext* RegisterFunctionContext(IFunctionContext * pFContext);
		virtual void UnregisterFunctionContext(IFunctionContext * pFContext, IFunctionContext * pParent);

		inline FlyerMap& GetFlyerMap(void)    //Flyer type-instance map
		{
			return m_FlyerMap;
		}

		inline IFunctionContext* FunctionContext()
		{
			return m_pCurrentContext;
		}

    private:

        IFunctionContext* m_pRootContext;
        IFunctionContext* m_pCurrentContext;        
        FlyerMap m_FlyerMap;

    };

}}//qor::detail

#endif//QOR_PP_H_FRAMEWORK_THREADCONTEXT
