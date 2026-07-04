// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/qor/essentials/reporting/ierrorreporting.h"
#include "src/qor/essentials/reporting/iassertionfailurereporting.h"
#include "src/qor/essentials/reporting/itestfailurereporting.h"

namespace qor{ 

    struct qor_pp_module_interface(QOR_ERROR) ReportingImpl : public IErrorReporting, public IAssertionFailureReporting, public ITestFailureReporting
    {
        virtual void note(const std::string& message);
        virtual void warning(const std::string& message);
        virtual void continuable(const std::string& message);
        virtual void serious(const std::string& message);
        virtual void fatal(const std::string& message);
        virtual void AssertionFailure(const std::string& message);
        virtual void TestFailure(const std::string& message);
    };

}//qor