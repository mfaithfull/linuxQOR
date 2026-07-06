// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#ifdef NDEBUG
#include <buildnumber.h>
#endif
#include "src/qor/module/module.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/framework/io/filesystem/ifilesystem.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/factory/internalfactory.h"
#include "src/qor/memory/injection/typeregentry.h"
#include "src/qor/memory/reference/newref.h"
#include "asyncioservice.h"
#include "iouringservice/iouringeventprocessor.h"
#include "iouringservice/iouringinitiator.h"

extern "C"
{
	qor::Module& ThisModule(void)
	{
		static qor::Module QORModule("Querysoft Open Runtime: Linux Async IO Service Module", qor_pp_module_ver_string );

		//Register the Linux specific implementations
		static qor::TypeRegEntry< qor::io::async::lin::Service, qor::io::async::Service > regAsyncIOService;
		static qor::TypeRegEntry< qor::io::async::lin::IOUringEventProcessor, qor::io::async::EventProcessor > regAsyncIOEventProcessor;
		static qor::TypeRegEntry< qor::io::async::lin::IOUringInitiator, qor::io::async::Initiator > regAsyncIOInitiator;
		return QORModule;
	}
}
