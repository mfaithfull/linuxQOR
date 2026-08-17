// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "host.h"

qor::Module& HostModule(void)
{
	static qor::Host host;
	return host;
}

