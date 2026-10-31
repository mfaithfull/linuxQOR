// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_API_DIRECT2D
#define QOR_PP_H_OS_WINDOWS_API_DIRECT2D

#include "src/platform/os/windows/api_layer/user/user32.h"

#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d2d1_2.h>
#include <d2d1_3.h>
#include <d2d1_3helper.h>
#include <d2d1effectauthor.h>
#include <d2d1effectauthor_1.h>
#include <d2d1effecthelpers.h>
//#include <d2d1effecthelpers.hlsli>
#include <d2d1effects.h>
#include <d2d1effects_1.h>
#include <d2d1effects_2.h>
#include <d2d1helper.h>
#include <d2d1svg.h>
#include <dcommon.h>

namespace qor { namespace win { namespace api {

	class qor_pp_module_interface(QOR_WINAPIDIRECT2D) Direct2D
	{
	public:

    };

}}}//qor::win::api

#endif//QOR_PP_H_OS_WINDOWS_API_DIRECT2D
