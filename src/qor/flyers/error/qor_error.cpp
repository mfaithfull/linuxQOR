// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#ifdef NDEBUG
#include <buildnumber.h>
#endif
#include "src/qor/module/module.h"
#include "reportingimpl.h"

extern "C"
{
	qor::Module& ThisModule(void)
	{
		static qor::Module QORModule("Querysoft Open Runtime: Error Handler Module", qor_pp_module_ver_string );

		static qor::ReportingImpl reportingImpl;
		qor::ErrorImpl()= dynamic_cast<qor::IErrorReporting*>(&reportingImpl);
		qor::AssertionFailureImpl() = dynamic_cast<qor::IAssertionFailureReporting*>(&reportingImpl);
		qor::TestFailureImpl() = dynamic_cast<qor::ITestFailureReporting*>(&reportingImpl);

		return QORModule;
	}
}
