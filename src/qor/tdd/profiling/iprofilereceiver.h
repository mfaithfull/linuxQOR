// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_IPROFILERECEIVER
#define QOR_PP_H_IPROFILERECEIVER

#include <chrono>
#include "src/qor/flyers/interception/ifunctioncontext.h"

namespace qor {

	struct IProfileReceiver
	{
		virtual void Profile(const std::chrono::duration<int64_t, std::milli>, IFunctionContext*) = 0;
	};

}//qor

#endif//QOR_PP_H_IPROFILERECEIVER
