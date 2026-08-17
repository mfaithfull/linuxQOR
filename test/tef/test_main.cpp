// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/tdd/test/test.h"
#include "src/qor/module/module.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/platform/platform.h"
#include "sdk/app.h"

qor_pp_module_requires(ICurrentThread)

using namespace qor;
using namespace qor::app;
using namespace qor::platform;

int main(int argc, const char** argv, char** env)
{
	return AppBuilder().Build<Application>("Test TEF")(qor_unlocked).SetRole<app::Role>().
        Run([argv, argc]()->int
            {
                return qor::test::Factory::Instance().main(argc, argv);
            });
}

qor_pp_export_non_msvc qor::Module& ThisModule(void)
{
	static qor::Module QORModule("Querysoft Open Runtime: Test TEF Module", qor_pp_module_ver_string );
	return QORModule;
}