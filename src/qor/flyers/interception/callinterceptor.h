// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_CALLINTERCEPTOR
#define QOR_PP_H_CALLINTERCEPTOR

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/essentials/objectcontext/typedany.h"
#include "src/qor/memory/reference/flyerref.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/datastructures/guid.h"
#include "callcontext.h"
#include "flyer.h"
#include "flystrapbase.h"

namespace qor {

    //Declare a GUID for CallInterceptor
    constexpr GUID CallInterceptorGUID = {0x00000001, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0}};
    class qor_pp_module_interface(QOR_INTERCEPTION) CallInterceptor;

    //Override the ref_of trait for CallInteceptor to make it use a FlyerRef
    template<> struct ref_of< CallInterceptor >
    {
        typedef qor::FlyerRef< CallInterceptor > type;
    };

    //Attach the CallIterceptor GUID to the CallInterceptor type with the guid_of trait
    template<> struct guid_of< CallInterceptor >
    {
        static const GUID* guid()
        {
            return &CallInterceptorGUID;            
        }
    };

    class qor_pp_module_interface(QOR_INTERCEPTION) CallInterceptor : public Flyer< CallInterceptor, FlyStrapBase >
    {
    public:
		CallInterceptor();
		virtual ~CallInterceptor();
		virtual void CallMade( CallContext* call, IFunctionContext* function );
		virtual void OnReturnAssignment( CallContext* call );
		virtual void OnReturn(CallContext* call);
		virtual void CallCompleted( CallContext* call );
    };

}//qor

#endif//QOR_PP_H_CALLINTERCEPTOR
