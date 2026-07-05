// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "host.h"

extern "C"
{
	qor::Module& ThisModule(void)
	{
		static qor::Host host;
		return host;
	}
}
