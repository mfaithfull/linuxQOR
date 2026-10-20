// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//qor_mime test. The test first plan is in memory-bank/qor_mime_plan.md:
//step 0 - the module is present in the executable image, its module singleton is
//registered on load, and the filter can be constructed.
//step 1 - mime_filter_passes_octets_through: the filter wired between a StringSource
//and a StringSink leaves the octet stream untouched (output == input). This proves
//the outline is correctly wired as a real pipeline filter before any detection is
//written. Detection is added one increment at a time after this.

#include "src/configuration/configuration.h"
#include <string>
#include "src/qor/tdd/tdd.h"
#include "src/framework/data/pipeline/pipeline.h"
#include "src/framework/data/pipeline/podbuffer.h"
#include "src/components/data/pipeline/sources/stringsource/stringsource.h"
#include "src/components/data/pipeline/sinks/stringsink/stringsink.h"
#include "src/framework/data/mime/mime.h"

using namespace qor;
using namespace qor::pipeline;
using namespace qor::pipeline::components;

qor_pp_test_case(mime_module_loads)
{
    //qor_mime.dll is linked and its module singleton is registered on load.
    //This confirms the module is present in the executable image and that the
    //filter can be built against qor_pipeline.
    qor::mime::MIMERecognizerFilter recognizer;
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::Unresolved).isTrue();
}

//Step 1 - the filter must be a real pipeline filter: it sits between a source and a
//sink and whatever arrives at the sink is exactly what left the source. No detection
//is involved yet; the octets are only required to pass through untouched.
qor_pp_test_case(mime_filter_passes_octets_through)
{
    //Push mode is the working pump path for a filter in the base framework: Source::Read
    //fills the source buffer, then Filter::Write runs WriteFilter and hands the units on
    //to the sink. The Pipeline parent carries the flow mode (Element::SetFlowMode only
    //forwards to a parent).
    std::string testData = "Mary had a little lamb. The lamb was white as snow.";

    Pipeline testPipeline;
    testPipeline.SetFlowMode(Element::FlowMode::Push);

    ByteBuffer inBuffer(128);
    ByteBuffer outBuffer(128);

    StringSource source;
    source.SetParent(&testPipeline);
    source.SetBuffer(&inBuffer);
    source.SetData(testData);

    StringSink sink;
    sink.SetParent(&testPipeline);
    sink.SetBuffer(&outBuffer);

    qor::mime::MIMERecognizerFilter recognizer;
    recognizer.SetParent(&testPipeline);
    //Filter::SetSource links the source's sink to the filter, Filter::SetSink links the
    //sink's source to the filter, so the chain is source -> recognizer -> sink.
    recognizer.SetSource(&source);
    recognizer.SetSink(&sink);

    size_t unitsPumped = 0;
    recognizer.Pump(unitsPumped, testData.length());

    qor_pp_assert_that(sink.GetData() == testData).isTrue();
}
