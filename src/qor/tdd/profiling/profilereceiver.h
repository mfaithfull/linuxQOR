// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROFILERECEIVER
#define QOR_PP_H_PROFILERECEIVER

#include <chrono>

#include "iprofilereceiver.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/essentials/objectcontext/typedany.h"
#include "src/qor/memory/reference/flyerref.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/datastructures/guid.h"
#include "src/qor/flyers/interception/callcontext.h"
#include "src/qor/flyers/interception/flyer.h"
#include "src/qor/flyers/interception/flystrapbase.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/informative.h"

namespace qor {

	class ProfileReceiver : public Flyer< ProfileReceiver, IProfileReceiver >
	{
     public:

        ProfileReceiver()
        {
            Flyer< ProfileReceiver, IProfileReceiver >::Push();
        }

		virtual ~ProfileReceiver()
        {
            Flyer< ProfileReceiver, IProfileReceiver >::Pop();
        }

		virtual void Profile(const std::chrono::duration<int64_t, std::milli> duration, IFunctionContext* fContext)
		{
		    issue<log::Informative, const std::string&>(std::format("Profile: {0}ms", duration), fContext);
		}
	};

    constexpr GUID ProfileReceiverGUID = {0x3078ff1b, 0x54d0, 0x44ee, {0x92, 0x7b, 0xac, 0x3f, 0xcf, 0x26, 0xb1, 0x6b}};
    qor_pp_declare_guid_of(ProfileReceiver,ProfileReceiverGUID)
    qor_pp_declare_ref_of(ProfileReceiver,FlyerRef);


}//qor

#endif//QOR_PP_H_PROFILERECEIVER
