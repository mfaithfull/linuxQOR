// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ERROR_HANDLER
#define QOR_PP_H_ERROR_HANDLER

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/essentials/objectcontext/typedany.h"
#include "src/qor/memory/reference/flyerref.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/datastructures/guid.h"
#include "src/qor/flyers/interception/callcontext.h"
#include "src/qor/flyers/interception/flyer.h"
#include "src/qor/flyers/interception/flystrapbase.h"
#include "errorbase.h"

namespace qor{ 

    class BaseIssueHandler;
    constexpr GUID BaseIssueHandlerGUID = {0x48467945, 0xfbae, 0x4a8d, { 0xb2, 0xbb, 0xd0, 0x50, 0x10, 0xde, 0x94, 0x02}};
    qor_pp_declare_guid_of(BaseIssueHandler,BaseIssueHandlerGUID);
    qor_pp_declare_ref_of(BaseIssueHandler,FlyerRef);
    
    class BaseIssueHandler
    {
    public:
        BaseIssueHandler() = default;
		virtual ~BaseIssueHandler() = default;      
    };
    
    template<class T>
    class IssueHandler : public Flyer< IssueHandler<T>, BaseIssueHandler >
    {
    public:

        IssueHandler()
        {
            Flyer< IssueHandler<T>, BaseIssueHandler >::Push();
        }

		virtual ~IssueHandler()
        {
            Flyer< IssueHandler<T>, BaseIssueHandler >::Pop();
        }

        virtual bool Handle(const T& /*Issue*/)
        {
            return false;//false here means failure to resovle the issue
        }        
    };    

    constexpr GUID SeverityIssueHandlerGUID = {0x88a20115, 0xb7cb, 0x4a45, { 0xa8, 0x8a, 0x72, 0xa3, 0x1b, 0x61, 0x0a, 0x3d}};
    qor_pp_declare_guid_of(IssueHandler<ErrorBase>,SeverityIssueHandlerGUID)
    qor_pp_declare_ref_of(IssueHandler<ErrorBase>,FlyerRef);

    template<class T> struct ref_of< IssueHandler<T> >
    {
        typedef qor::FlyerRef< IssueHandler<T> > type;
    };

    constexpr GUID FatalIssueHandlerGUID = {0xdfa9fac1, 0x145d, 0x4c3e, { 0x90, 0x19, 0x83, 0x0b, 0xaf, 0x2c, 0xa4, 0xda}};
    qor_pp_declare_guid_of(IssueHandler<Fatal>,FatalIssueHandlerGUID)
    qor_pp_declare_ref_of(IssueHandler<Fatal>,FlyerRef);
    
    constexpr GUID SeriousIssueHandlerGUID = {0x8f4bafe3, 0x6db2, 0x4df5, { 0x82, 0x14, 0x30, 0x73, 0xe6, 0xd9, 0x6b, 0x3e}};
    qor_pp_declare_guid_of(IssueHandler<Serious>,SeriousIssueHandlerGUID)
    qor_pp_declare_ref_of(IssueHandler<Serious>,FlyerRef);

    constexpr GUID ContinuableIssueHandlerGUID = {0x311e5218, 0x2683, 0x4ae1, { 0xa1, 0xcc, 0x9b, 0x23, 0xce, 0x13, 0xec, 0x26}};
    qor_pp_declare_guid_of(IssueHandler<Continuable>,ContinuableIssueHandlerGUID)
    qor_pp_declare_ref_of(IssueHandler<Continuable>,FlyerRef);

    constexpr GUID WarningIssueHandlerGUID = {0x4f9f3ae7, 0x4e36, 0x45b2, { 0x83, 0xcf, 0x17, 0xe2, 0x5f, 0x1f, 0x84, 0x43}};
    qor_pp_declare_guid_of(IssueHandler<Warning>,WarningIssueHandlerGUID)
    qor_pp_declare_ref_of(IssueHandler<Warning>,FlyerRef);

    constexpr GUID NoteIssueHandlerGUID = {0xecee7a82, 0x239b, 0x4ef6, { 0xb6, 0x21, 0x45, 0x59, 0x50, 0x62, 0x84, 0x72}};
    qor_pp_declare_guid_of(IssueHandler<Note>,NoteIssueHandlerGUID)
    qor_pp_declare_ref_of(IssueHandler<Note>,FlyerRef);
}//qor

#endif//QOR_PP_H_ERROR_HANDLER
