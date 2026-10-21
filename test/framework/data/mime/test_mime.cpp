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
//step 4 - mime_recognises_jpeg_signature: the second detection increment. A stream whose
//leading octets are the three byte JPEG magic FF D8 FF must be classified as image/jpeg.
//step 5 - mime_recognises_gif_signature: the third detection increment. A stream whose
//leading octets are the six byte GIF magic GIF89a or GIF87a must be classified as image/gif.
//step 6 - mime_recognises_pdf_signature: the fourth detection increment. A stream whose
//leading octets are the four byte PDF file magic %PDF must be classified as application/pdf.
//step 7 - mime_recognises_zip_signature: the fifth detection increment. A stream whose
//leading octets are the four byte ZIP local file header magic PK 03 04 must be classified
//as application/zip.
//step 8 - mime_recognises_xml_signature: the sixth detection increment. A stream whose
//leading octets are the five character XML declaration marker <?xml must be classified as
//application/xml. It is a strong text heuristic rather than a fixed binary magic, so it is
//evaluated after the definite binary signatures in the fixed order (png, jpeg, gif, pdf, zip,
//xml, ...).
//step 9 - mime_recognises_html_signature: the seventh detection increment. A stream that is
//an HTML document must be classified as text/html: GetDetectedType() == MIMEType::HTML and
//GetTypeName() == "text/html". HTML has no binary magic, so there are two text heuristics and
//the test asserts both, one stream each with fresh elements for the second: the first stream
//opens with the HTML5 doctype <!DOCTYPE html> (its spelling is case-insensitive) and the
//second stream has no doctype at all, opening instead with the document-level tag <HTML>,
//which the first-tag check (the first '<' starts a tag named html, head, body or title) must
//also accept. Evaluated after the XML marker in the fixed order (png, jpeg, gif, pdf, zip,
//xml, html, ...).
//step 10 - mime_recognises_json_signature: the eighth detection increment. A stream that is
//a JSON document must be classified as application/json: GetDetectedType() == MIMEType::JSON and
//GetTypeName() == "application/json". JSON has no magic number, so this is a text heuristic - the
//first octet that is not JSON white space (space, tab, line feed and carriage return, the only four
//characters JSON allows as whitespace) must open a container: the object brace or the array bracket.
//The test asserts both container forms, one stream each with fresh elements for the second, and the
//second stream carries leading whitespace so the skip-past-whitespace half of the heuristic is
//exercised. Evaluated after the HTML markup checks in the fixed order (png, jpeg, gif, pdf, zip,
//xml, html, json, ...).



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


//Step 4 - the second detection increment. A stream whose leading octets are the three byte
//JPEG file magic FF D8 FF must be classified as image/jpeg: GetDetectedType() == MIMEType::JPEG
//and GetTypeName() == "image/jpeg". The JPEG signature is shorter than PNG's, so this increment
//also proves the classification guards are per signature length (three octets are enough here).
//As in step 3 the pass-through contract is re-checked, so adding a detector cannot break the
//octet stream proven in steps 1 and 2.
qor_pp_test_case(mime_recognises_jpeg_signature)
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

    //The three byte JPEG file signature FF D8 FF, then a JFIF style marker (E0) and text body
    //so the stream is longer than the signature alone.
    std::string jpegData;
    jpegData.push_back(static_cast<char>(0xFF));
    jpegData.push_back(static_cast<char>(0xD8));
    jpegData.push_back(static_cast<char>(0xFF));
    jpegData.push_back(static_cast<char>(0xE0));
    jpegData += "JFIF payload bytes for the recognizer test stream.";

    source.SetData(jpegData);

    size_t unitsPumped = 0;
    recognizer.Pump(unitsPumped, jpegData.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink.GetData() == jpegData).isTrue();

    //The captured signature is the JPEG magic, so the stream is a JPEG.
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::JPEG).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "image/jpeg").isTrue();
}


//Step 5 - the third detection increment. A stream whose leading octets are the six byte
//GIF file magic GIF89a or GIF87a must be classified as image/gif: GetDetectedType() ==
//MIMEType::GIF and GetTypeName() == "image/gif". The GIF spec defines both header variants
//so both are asserted, one stream each, using fresh elements for the second (Reset() reuse
//is plan step 15 and is not exercised here). As in steps 3 and 4 the pass-through contract
//is re-checked so that adding a detector cannot break the octet stream proven earlier.
//This case is written FIRST and seen RED - Inspect() has no GIF detector yet.
qor_pp_test_case(mime_recognises_gif_signature)
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

    //The six byte GIF89a file signature followed by a short body so that the stream is
    //longer than the header alone.
    std::string gif89Data;
    gif89Data += "GIF89a";
    gif89Data += " payload bytes for the recognizer test stream.";

    source.SetData(gif89Data);

    size_t unitsPumped = 0;
    recognizer.Pump(unitsPumped, gif89Data.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink.GetData() == gif89Data).isTrue();

    //The captured signature is the GIF89a magic, so the stream is a GIF.
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::GIF).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "image/gif").isTrue();

    //The GIF87a variant, second stream with fresh elements.
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

    std::string gif87Data;
    gif87Data += "GIF87a";
    gif87Data += " payload bytes for the recognizer test stream.";

    source2.SetData(gif87Data);

    size_t unitsPumped2 = 0;
    recognizer2.Pump(unitsPumped2, gif87Data.length());

    qor_pp_assert_that(sink2.GetData() == gif87Data).isTrue();
    qor_pp_assert_that(recognizer2.GetDetectedType() == qor::mime::MIMEType::GIF).isTrue();
    qor_pp_assert_that(std::string(recognizer2.GetTypeName()) == "image/gif").isTrue();
}


//Step 6 - the fourth detection increment. A stream whose leading octets are the four byte
//PDF file magic %PDF must be classified as application/pdf: GetDetectedType() ==
//MIMEType::PDF and GetTypeName() == "application/pdf". The PDF header is followed by the
//version marker in real files, so the stream carries "%PDF-1.4" plus a short body. As in
//steps 3-5 the pass-through contract is re-checked so that adding a detector cannot break
//the octet stream proven earlier. This case is written FIRST and seen RED - Inspect() has
//no PDF detector yet.
qor_pp_test_case(mime_recognises_pdf_signature)
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

    //The four byte PDF file signature, the version marker, and a short body so that the
    //stream is longer than the signature alone.
    std::string pdfData;
    pdfData += "%PDF-1.4\n";
    pdfData += " payload bytes for the recognizer test stream.";

    source.SetData(pdfData);

    size_t unitsPumped = 0;
    recognizer.Pump(unitsPumped, pdfData.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink.GetData() == pdfData).isTrue();

    //The captured signature is the PDF magic, so the stream is a PDF.
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::PDF).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "application/pdf").isTrue();
}


//Step 7 - the fifth detection increment. A stream whose leading octets are the four byte
//ZIP local file header magic PK 03 04 must be classified as application/zip: GetDetectedType()
//== MIMEType::ZIP and GetTypeName() == "application/zip". The magic is the ASCII characters
//'P' 'K' followed by the format version numbers 03 04, so the stream carries those raw octets
//plus a short body so that it is longer than the signature alone. As in steps 3-6 the pass-through
//contract is re-checked so that adding a detector cannot break the octet stream proven earlier.
//This case is written FIRST and seen RED - Inspect() has no ZIP detector yet.
qor_pp_test_case(mime_recognises_zip_signature)
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

    //The four byte ZIP signature ('P' 'K' 03 04) and a short body so that the stream is
    //longer than the signature alone.
    std::string zipData;
    zipData += "PK\x03\x04";
    zipData += " payload bytes for the recognizer test stream.";

    source.SetData(zipData);

    size_t unitsPumped = 0;
    recognizer.Pump(unitsPumped, zipData.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink.GetData() == zipData).isTrue();

    //The captured signature is the ZIP magic, so the stream is a ZIP archive.
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::ZIP).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "application/zip").isTrue();
}


//Step 8 - the sixth detection increment. A stream whose leading octets are the five character
//XML declaration marker <?xml must be classified as application/xml: GetDetectedType() ==
//MIMEType::XML and GetTypeName() == "application/xml". Unlike the fixed binary magics above,
//XML has no binary magic number and is recognised from the text of its optional declaration,
//so this is a strong text heuristic. The marker is the characters '<' '?' 'x' 'm' 'l' (0x3C
//0x3F 0x78 0x6D 0x6C); the stream carries that marker followed by the rest of a declaration
//and a short body so that it is longer than the marker alone. As in steps 3-7 the pass-through
//contract is re-checked so that adding a detector cannot break the octet stream proven earlier.
//This case is written FIRST and seen RED - Inspect() has no XML detector yet.
qor_pp_test_case(mime_recognises_xml_signature)
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

    //The five character XML declaration marker and a short body so that the stream is longer
    //than the signature alone.
    std::string xmlData;
    xmlData += "<?xml version=\"1.0\"?>\n";
    xmlData += "<root>payload bytes for the recognizer test stream.</root>";

    source.SetData(xmlData);

    size_t unitsPumped = 0;
    recognizer.Pump(unitsPumped, xmlData.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink.GetData() == xmlData).isTrue();

    //The captured signature is the XML declaration marker, so the stream is XML.
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::XML).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "application/xml").isTrue();
}


//Step 9 - the seventh detection increment. A stream that is an HTML document must be
//classified as text/html: GetDetectedType() == MIMEType::HTML and GetTypeName() ==
//"text/html". HTML has no binary magic, so recognition is a pair of text heuristics: the
//document opens with the doctype declaration <!DOCTYPE html ...> whose spelling is case-
//insensitive, or, when no doctype is present, the first tag is a document-level tag named
//html, head, body or title. The test asserts both paths, one stream each, using fresh
//elements for the second (as in step 5's two GIF variants; Reset() reuse is plan step 15
//and is not exercised here). The first stream carries the lower case doctype so the case-
//insensitive comparison is exercised, the second stream carries no doctype and opens with
//the upper case tag <HTML> so the first-tag path is exercised with a case-folded name. As
//in steps 3-8 the pass-through contract is re-checked so that adding a detector cannot
//break the octet stream proven earlier. This case is written FIRST and seen RED -
//Inspect() has no HTML detector yet.
qor_pp_test_case(mime_recognises_html_signature)
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

    //The lower case HTML5 doctype followed by the start of a document and a short body
    //so that the stream is longer than the doctype alone.
    std::string htmlDoctypeData;
    htmlDoctypeData += "<!DOCTYPE html>\n";
    htmlDoctypeData += "<html><head><title>qor mime test</title></head><body>payload bytes.</body></html>";

    source.SetData(htmlDoctypeData);

    size_t unitsPumped = 0;
    recognizer.Pump(unitsPumped, htmlDoctypeData.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink.GetData() == htmlDoctypeData).isTrue();

    //The captured signature is the HTML5 doctype, so the stream is HTML.
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::HTML).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "text/html").isTrue();

    //The second stream: no doctype at all, the document opens with the upper case
    //document-level tag <HTML>. Fresh elements for the second stream.
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

    std::string htmlTagData;
    htmlTagData += "<HTML>\n<head><title>qor mime test</title></head><body>payload bytes.</body></html>";

    source2.SetData(htmlTagData);

    size_t unitsPumped2 = 0;
    recognizer2.Pump(unitsPumped2, htmlTagData.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink2.GetData() == htmlTagData).isTrue();

    //The captured signature is a document-level tag, so the stream is HTML.
    qor_pp_assert_that(recognizer2.GetDetectedType() == qor::mime::MIMEType::HTML).isTrue();
    qor_pp_assert_that(std::string(recognizer2.GetTypeName()) == "text/html").isTrue();
}

//Step 10 - the eighth detection increment. A stream that is a JSON document must be classified as
//application/json: GetDetectedType() == MIMEType::JSON and GetTypeName() == "application/json".
//JSON has no magic number, so this is a text heuristic - the first octet that is not JSON white
//space (space, tab, line feed and carriage return, the only four characters JSON allows as
//whitespace) must open a container: the object brace or the array bracket. The test asserts both
//container forms, one stream each, using fresh elements for the second (as in step 5's two GIF
//variants and step 9's two HTML variants; Reset() reuse is plan step 15 and is not exercised here).
//The first stream is an object with no leading whitespace so the brace decides immediately, and the
//second stream is an array preceded by spaces, a line feed and a tab, so the skip-past-whitespace
//half of the heuristic is exercised. As in steps 3-9 the pass-through contract is re-checked so that
//adding a detector cannot break the octet stream proven earlier. This case is written FIRST and
//seen RED - Inspect() has no JSON detector yet.
qor_pp_test_case(mime_recognises_json_signature)
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

    //A JSON object: the opening brace is the first octet, so the heuristic has nothing to skip.
    //The body is short but long enough that the stream exceeds the signature window.
    std::string jsonObjectData;
    jsonObjectData += "{ \"qor\" : \"mime test\", \"step\" : 10 }\n";
    jsonObjectData += "{\"padding\":\"payload octets so the stream exceeds the signature window\"}";

    source.SetData(jsonObjectData);

    size_t unitsPumped = 0;
    recognizer.Pump(unitsPumped, jsonObjectData.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink.GetData() == jsonObjectData).isTrue();

    //The first non-space octet is the object brace, so the stream is JSON.
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::JSON).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "application/json").isTrue();

    //The second stream: a JSON array whose bracket is preceded by JSON whitespace - three spaces,
    //a line feed and a tab - so the heuristic has to skip whitespace before it can decide. Fresh
    //elements, as for the second stream in steps 2, 5 and 9.
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

    std::string jsonArrayData;
    jsonArrayData += "   \n\t[ 1, 2, 3 ]\n";
    jsonArrayData += "[\"padding\",\"payload octets so the stream exceeds the signature window\"]";

    source2.SetData(jsonArrayData);

    size_t unitsPumped2 = 0;
    recognizer2.Pump(unitsPumped2, jsonArrayData.length());

    //The recognizer must not alter the octet stream, leading whitespace included.
    qor_pp_assert_that(sink2.GetData() == jsonArrayData).isTrue();

    //The first non-space octet is the array bracket, so the stream is JSON.
    qor_pp_assert_that(recognizer2.GetDetectedType() == qor::mime::MIMEType::JSON).isTrue();
    qor_pp_assert_that(std::string(recognizer2.GetTypeName()) == "application/json").isTrue();
}


