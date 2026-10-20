// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//qor_mime test. Step 0 of the test first plan in memory-bank/qor_mime_plan.md:
//the module is present in the executable image, its module singleton is registered
//on load, and the filter can be constructed. Detection is added one increment at a
//time after this.

#include "src/configuration/configuration.h"
#include "src/qor/tdd/tdd.h"
#include "src/framework/data/mime/mime.h"

using namespace qor;

qor_pp_test_case(mime_module_loads)
{
    //qor_mime.dll is linked and its module singleton is registered on load.
    //This confirms the module is present in the executable image and that the
    //filter can be built against qor_pipeline.
    qor::mime::MIMERecognizerFilter recognizer;
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::Unresolved);
}
