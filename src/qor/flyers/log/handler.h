// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_LOG_HANDLER
#define QOR_PP_H_LOG_HANDLER

#include "src/framework/parallel/thread/thread.h"
#include "src/qor/essentials/objectcontext/typedany.h"
#include "src/qor/memory/reference/flyerref.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/datastructures/guid.h"
#include "src/qor/flyers/interception/callcontext.h"
#include "src/qor/flyers/interception/flyer.h"
#include "src/qor/flyers/interception/flystrapbase.h"
#include "src/qor/flyers/error/handler.h"
#include "log.h"
#include "debug.h"
#include "informative.h"
#include "important.h"
#include "impactful.h"
#include "imperative.h"

namespace qor{

    constexpr GUID LogIssueHandlerGUID = {0xe45ea271, 0x7394, 0x42d2, {0x8f, 0xb8, 0xec, 0x2a, 0x08, 0x5b, 0x2b, 0xec}};
    qor_pp_declare_guid_of(IssueHandler<log::Log>,LogIssueHandlerGUID)
    qor_pp_declare_ref_of(IssueHandler<log::Log>,FlyerRef);

    constexpr GUID DebugLogHandlerGUID = {0x12637a5c, 0x5ed4, 0x431a, {0xaf, 0x64, 0x70, 0x0a, 0x0e, 0x63, 0xfc, 0x20}};
    qor_pp_declare_guid_of(IssueHandler<log::Debug>,DebugLogHandlerGUID)
    qor_pp_declare_ref_of(IssueHandler<log::Debug>,FlyerRef);
    
    constexpr GUID InformativeLogHandlerGUID = {0x75b73265, 0x75f1, 0x434f, {0x89, 0xcf, 0x76, 0x1d, 0x3c, 0x1a, 0xf4, 0x0d}};
    qor_pp_declare_guid_of(IssueHandler<log::Informative>,InformativeLogHandlerGUID)
    qor_pp_declare_ref_of(IssueHandler<log::Informative>,FlyerRef);

    constexpr GUID ImportantLogHandlerGUID = {0xb75b9b44, 0x1b5b, 0x4a26, {0x9f, 0x13, 0xb6, 0x58, 0xfc, 0x6b, 0x19, 0xf7}};
    qor_pp_declare_guid_of(IssueHandler<log::Important>,ImportantLogHandlerGUID)
    qor_pp_declare_ref_of(IssueHandler<log::Important>,FlyerRef);

    constexpr GUID ImpactfulLogHandlerGUID = {0x7f96ad25, 0xac48, 0x4536, {0xbd, 0x55, 0xbf, 0x17, 0x8f, 0x5e, 0xd7, 0x5d}};
    qor_pp_declare_guid_of(IssueHandler<log::Impactful>,ImpactfulLogHandlerGUID)
    qor_pp_declare_ref_of(IssueHandler<log::Impactful>,FlyerRef);
    
    constexpr GUID ImperativeLogHandlerGUID = {0x34f13609, 0x7100, 0x4093, {0x95, 0xe0, 0x1d, 0xf9, 0x02, 0x0f, 0x12, 0x90}};
    qor_pp_declare_guid_of(IssueHandler<log::Imperative>,ImperativeLogHandlerGUID)
    qor_pp_declare_ref_of(IssueHandler<log::Imperative>,FlyerRef);

}//qor

#endif//QOR_PP_H_LOG_HANDLER
