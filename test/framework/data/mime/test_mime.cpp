// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//qor_mime test. The test first plan is in memory-bank/qor_mime_plan.md:
//step 0 - the module is present in the executable image, its module singleton is
//registered on load, and the filter can be constructed.
//step 1 - mime_filter_passes_octets_through: the filter wired between a StringSource
//and a StringSink leaves the octet stream untouched (output == input). This proves
//the outline is correctly wired as a real pipeline filter before any detection is
//written. Detection is added one increment at a time after this.
//step 2 - mime_captures_leading_octets: the filter captures the leading octets of the
//stream into its 16 byte signature window without consuming them (40 byte input -> 16
//captured, 4 byte input -> 4 captured). Still no type detection.
//step 3 - mime_recognises_png_signature: the first detection increment. A stream whose
//leading octets are the eight byte PNG magic 89 50 4E 47 0D 0A 1A 0A must be classified
//as image/png: GetDetectedType() == MIMEType::PNG and GetTypeName() == "image/png".
//Detection is added one type at a time (see memory-bank/qor_mime_plan.md).

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

//Step 2 - the recognizer must capture the leading octets of the stream that passes through
//it into its signature window without consuming them (the octets still have to reach the
//sink, which step 1 proved). The window is SIGNATURE_WINDOW_BYTES wide, which is sixteen,
//so a 40 byte input yields exactly 16 captured octets and a 4 byte input yields exactly
//4 - the capture is capped by the window and by the stream length. No type detection is
//involved yet; that comes one type at a time from step 3.
qor_pp_test_case(mime_captures_leading_octets)
{
    //Push-mode wiring is the working pump path for a filter (see step 1 notes).
    Pipeline testPipeline;
    testPipeline.SetFlowMode(Element::FlowMode::Push);

    ByteBuffer inBuffer(128);
    ByteBuffer outBuffer(128);

    StringSource source;
    source.SetParent(&testPipeline);
    source.SetBuffer(&inBuffer);

    StringSink sink;
    sink.SetParent(&testPipeline);
    sink.SetBuffer(&outBuffer);

    qor::mime::MIMERecognizerFilter recognizer;
    recognizer.SetParent(&testPipeline);
    recognizer.SetSource(&source);
    recognizer.SetSink(&sink);

    std::string longData(40,'A');
    source.SetData(longData);

    size_t unitsPumped = 0;
    recognizer.Pump(unitsPumped, longData.length());

    //Forty octets available, sixteen byte window - sixteen captured.
    qor_pp_assert_that(recognizer.GetSignatureByteCount() == 16).isTrue();

    //A short stream, fresh elements so that Reset() reuse is not being exercised here
    //(that is plan step 15). Four octets available, window has room - four captured.
    ByteBuffer inBuffer2(128);
    ByteBuffer outBuffer2(128);

    StringSource source2;
    source2.SetParent(&testPipeline);
    source2.SetBuffer(&inBuffer2);

    StringSink sink2;
    sink2.SetParent(&testPipeline);
    sink2.SetBuffer(&outBuffer2);

    qor::mime::MIMERecognizerFilter recognizer2;
    recognizer2.SetParent(&testPipeline);
    recognizer2.SetSource(&source2);
    recognizer2.SetSink(&sink2);

    std::string shortData(4,'B');
    source2.SetData(shortData);

    size_t unitsPumped2 = 0;
    recognizer2.Pump(unitsPumped2, shortData.length());

    qor_pp_assert_that(recognizer2.GetSignatureByteCount() == 4).isTrue();
}

//Step 3 - the first detection increment. A stream whose leading octets are the eight byte
//PNG file magic 89 50 4E 47 0D 0A 1A 0A must be classified as image/png: GetDetectedType()
//== MIMEType::PNG and GetTypeName() == "image/png". The pipeline contract is re-checked in
//this case (the octets must still reach the sink untouched) so that adding a detector cannot
//break the pass-through proven in steps 1 and 2.
qor_pp_test_case(mime_recognises_png_signature)
{
    //Push-mode wiring is the working pump path for a filter (see step 1 notes).
    Pipeline testPipeline;
    testPipeline.SetFlowMode(Element::FlowMode::Push);

    ByteBuffer inBuffer(128);
    ByteBuffer outBuffer(128);

    StringSource source;
    source.SetParent(&testPipeline);
    source.SetBuffer(&inBuffer);

    StringSink sink;
    sink.SetParent(&testPipeline);
    sink.SetBuffer(&outBuffer);

    qor::mime::MIMERecognizerFilter recognizer;
    recognizer.SetParent(&testPipeline);
    recognizer.SetSource(&source);
    recognizer.SetSink(&sink);

    //The eight byte PNG file signature followed by a short body so that the stream is
    //longer than the header alone.
    std::string pngData;
    pngData.push_back(static_cast<char>(0x89));
    pngData.push_back(static_cast<char>(0x50));
    pngData.push_back(static_cast<char>(0x4E));
    pngData.push_back(static_cast<char>(0x47));
    pngData.push_back(static_cast<char>(0x0D));
    pngData.push_back(static_cast<char>(0x0A));
    pngData.push_back(static_cast<char>(0x1A));
    pngData.push_back(static_cast<char>(0x0A));
    pngData += "IHDR payload bytes for the recognizer test stream.";

    source.SetData(pngData);

    size_t unitsPumped = 0;
    recognizer.Pump(unitsPumped, pngData.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink.GetData() == pngData).isTrue();

    //The captured signature is the PNG magic, so the stream is a PNG.
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::PNG).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "image/png").isTrue();
}

