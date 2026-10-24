// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//qor_mime test. The test first plan is in docs/archive/qor_mime_plan.md:
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
//Detection is added one type at a time (see docs/archive/qor_mime_plan.md).
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
//step 11 - mime_recognises_javascript_signature: the ninth detection increment. A stream that is a
//JavaScript source file must be classified as application/javascript: GetDetectedType() ==
//MIMEType::JavaScript and GetTypeName() == "application/javascript". JavaScript has no magic number,
//so this is a weak text heuristic with three leading signatures recognised in this increment: a
//shebang "#!" (the interpreter line leading a standalone script), a top-level "function " declaration,
//and a leading source comment "//" or "/*". The test asserts three streams, one per signature, each
//with fresh pipeline elements (the established shape of the multi-stream cases in steps 5, 9 and 10).
//JavaScript is case-sensitive, so the "function" compare in the recognizer is a plain byte compare,
//not the case-folded markupMatch used for the HTML markup above. Evaluated after the JSON container
//check in the fixed order (png, jpeg, gif, pdf, zip, xml, html, json, javascript, ...).
//step 12 - mime_recognises_plain_text_signature: the tenth detection increment. A stream that is a
//plain text document must be classified as text/plain: GetDetectedType() == MIMEType::PlainText and
//GetTypeName() == "text/plain". Plain text has no magic number, no markup marker and no container
//opener, so it is the default for text - the whole signature window is captured and every octet in
//it is printable: printable ASCII (0x20-0x7E including the space), the three controls the text and
//JSON conventions allow (tab, line feed and carriage return) and the high octets (0x80-0xFF) that
//carry UTF-8 encoded text. Any other control octet means the stream is not plain text and leaves the
//decision to the octet-stream fallback (plan step 13). The guard is evaluated after every definite
//signature in the fixed order (png, jpeg, gif, pdf, zip, xml, html, json, javascript, plain text) so
//the binary, markup, container and source streams recognised in the earlier increments are never
//reclassified. The test asserts two streams, one per half of the printable range, each with fresh
//pipeline elements (the established shape of the multi-stream cases in steps 5, 9, 10 and 11): stream
//1 an ASCII-only prose document, stream 2 a document carrying UTF-8 multi-byte octets. This case is
//written FIRST and seen RED - Inspect() has no plain text detector yet.
//step 13 - mime_falls_back_to_octet_stream: the fallback increment. A stream that carries a full
//signature window and matches none of the ten recognised signatures must be classified as
//application/octet-stream: GetDetectedType() == MIMEType::OctetStream and GetTypeName() ==
//"application/octet-stream". The fallback has no signature of its own - it is the eleventh and last
//guard in the fixed order (png, jpeg, gif, pdf, zip, xml, html, json, javascript, plain text, octet
//stream) and it fires only when every guard above has declined, which makes the recognizer total:
//once the window is full every stream ends with a name. Both streams are longer than the sixteen byte
//window so the whole window is captured - a partially captured window is no evidence at all and stays
//Unresolved until the octets that decide it arrive. Two streams are asserted, each with fresh pipeline
//elements (the established shape of the multi-stream cases in steps 5, 9, 10, 11 and 12): stream 1 is
//a raw binary blob opening with a NUL, so it carries no signature and the plain text guard rejects it;
//stream 2 opens with the printable 'M' 'Z' prefix of a Windows executable image but carries NUL octets
//inside the window, so a stream that looks like text at its very start is still not left unresolved.
//As in steps 3-12 the pass-through contract is re-checked so that adding the fallback cannot break the
//octet stream proven earlier. This case is written FIRST and seen RED - Inspect() has no fallback yet,
//so an unmatched stream is still reported as Unresolved.
//step 14 - mime_recognises_type_split_across_chunks: the chunking increment. Recognition must not depend
//on how the stream was chunked. A signature longer than a pump chunk is split across pumped chunks - the
//eight byte PNG magic does not fit in a three byte chunk, so it arrives across three chunks (three octets,
//then three more, then the last two with the payload octets that follow them). The signature window
//accumulates across chunks (m_signatureCount is a member and Inspect() runs again on every chunk), so the
//window must end full at sixteen octets and the stream must be classified image/png: GetDetectedType() ==
//MIMEType::PNG and GetTypeName() == "image/png", with the pass-through contract re-checked. This case is
//written FIRST and no implementation change is expected - the window already accumulates across chunks, so
//the case proves that claim rather than driving new code.
//step 15 - mime_recognizer_resets_between_streams: the reuse increment. One recognizer instance must be
//able to serve more than one stream. After Reset() the detected type and the captured signature must both
//be discarded: GetDetectedType() == MIMEType::Unresolved, GetTypeName() == "" and GetSignatureByteCount()
//== 0. The stream that follows must then be recognised from scratch rather than carried over, so the first
//stream here is the PNG stream of step 3 (the state being cleared is a KNOWN type, not the initial state)
//and the second is the PDF stream of step 6. Both streams use fresh pipeline elements (the established
//shape of the multi-stream cases in steps 5, 9, 10, 11, 12 and 13) with the ONE recognizer re-wired to
//them - the reuse under test is the recognizer, not the plumbing. Pass-through is re-checked per stream as
//in steps 3-14. This case is written FIRST; no implementation change is expected - Reset() is already
//implemented in mime.cpp, so the case verifies existing behaviour.

//step 16 - mime_recogniser_chained_with_qor_base64encodefilter: the recognizer must co-operate with
//another filter in the same pipeline. Two filters in series: StringSource -> MIMERecognizerFilter
//-> Base64EncodeFilter -> StringSink. The Base64EncodeFilter is an InlineFilter<byte> - a Buffer
//pair, not an Element - so it cannot sit in the element chain; the framework inserts it as the
//buffer shared by the two elements either side of it (see Pipeline::InsertInlineFilter, BeforeSink),
//i.e. the buffer of the sink. Raw octets written into its upstream side are encoded into its
//downstream side, and InlineFilter::WriteAcknowledge rewrites the item count it is handed to the
//number of units actually produced - the encoded count - which is exactly what lets an expanding
//filter hand more units to the sink than the source pushed (TransferOctets propagates it). Octet
//width expectations: byte unit size, and the encode filter pads per call, so the stream must be
//pushed in whole chunks whose length is a multiple of three; the filter must also be built with an
//explicit capacity because its default ctor has zero items. The stream here is the PNG magic plus a
//ten byte payload - 18 octets, a multiple of three, one pumped chunk, longer than the 16 byte
//window - pumped for the encoded length 24. Asserts: the sink text is exactly the base64 of the
//stream, the signature window is full (GetSignatureByteCount() == 16), and the recognizer still
//classifies the raw stream it inspects: MIMEType::PNG / "image/png". No implementation change is
//expected - the recognizer passes octets through untouched - the case proves the existing module
//composes with another filter.
//step 17 - mime_type_name_drives_a_sink: the optional application-level glue increment. The recognizer's registered name is used by the test application to choose which StringSink receives the stream: "image/png" routes to an image sink and "text/plain" routes to a text sink. qor_mime supplies the name only; the routing is local glue in the test, so no qor_mime implementation change is expected. The case asserts the registered names and that only the sink selected by the recognised name receives the stream.




#include "src/configuration/configuration.h"
#include <string>
#include "src/qor/tdd/tdd.h"
#include "src/framework/data/pipeline/pipeline.h"
#include "src/framework/data/pipeline/podbuffer.h"
#include "src/components/data/pipeline/sources/stringsource/stringsource.h"
#include "src/components/data/pipeline/sinks/stringsink/stringsink.h"
#include "src/framework/data/mime/mime.h"
#include "src/components/data/pipeline/filters/base64encodefilter/base64encodefilter.h"

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

//Step 11 - the ninth detection increment. A JavaScript source file must be classified as
//application/javascript. JavaScript has no magic number, so this is a weak text heuristic. Three
//leading signatures are recognised in this increment, each a strong enough indicator of a JavaScript
//source to classify the stream: (1) a shebang "#!" - the interpreter line that leads a standalone
//script file; (2) a top-level "function " declaration; (3) a leading source comment "//" or "/*" - a
//stream that begins with a source comment is a source file, and within the set of types the recognizer
//handles that means JavaScript. The test asserts three streams, one per signature, each with fresh
//pipeline elements (the established shape of the multi-stream cases in steps 5, 9 and 10). JavaScript
//is case-sensitive, so the "function" compare in the recognizer is a plain byte compare rather than
//the case-folded markupMatch used for the HTML markup above. Evaluated after the JSON container check
//in the fixed order. This case is written FIRST and seen RED - Inspect() has no JavaScript detector yet.
qor_pp_test_case(mime_recognises_javascript_signature)
{
    //Stream 1 - a standalone script leading with the shebang "#!" (0x23 0x21).
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

    std::string shebangData;
    shebangData += "#!/usr/bin/env node\n";
    shebangData += "console.log(\"qor mime recognises a shebang-led JavaScript file\");";

    source.SetData(shebangData);

    size_t unitsPumped = 0;
    recognizer.Pump(unitsPumped, shebangData.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink.GetData() == shebangData).isTrue();

    //The leading hash-bang makes the stream JavaScript.
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::JavaScript).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "application/javascript").isTrue();

    //Stream 2 - a source file opening with a top-level "function " declaration. Fresh elements.
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

    std::string functionData;
    functionData += "function greet(name) { return \"hello, \" + name; }\n";
    functionData += "greet(\"qor\");";

    source2.SetData(functionData);

    size_t unitsPumped2 = 0;
    recognizer2.Pump(unitsPumped2, functionData.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink2.GetData() == functionData).isTrue();

    //The leading ECMA keyword makes the stream JavaScript.
    qor_pp_assert_that(recognizer2.GetDetectedType() == qor::mime::MIMEType::JavaScript).isTrue();
    qor_pp_assert_that(std::string(recognizer2.GetTypeName()) == "application/javascript").isTrue();

    //Stream 3 - a source file opening with a block comment "/*". Fresh elements.
    ByteBuffer inBuffer3(128);
    ByteBuffer outBuffer3(128);

    StringSource source3;
    source3.SetParent(&testPipeline);
    source3.SetBuffer(&inBuffer3);

    StringSink sink3;
    sink3.SetParent(&testPipeline);
    sink3.SetBuffer(&outBuffer3);

    qor::mime::MIMERecognizerFilter recognizer3;
    recognizer3.SetParent(&testPipeline);
    recognizer3.SetSource(&source3);
    recognizer3.SetSink(&sink3);

    std::string commentData;
    commentData += "/* qor mime JavaScript test */\n";
    commentData += "var greeting = \"hello\";";

    source3.SetData(commentData);

    size_t unitsPumped3 = 0;
    recognizer3.Pump(unitsPumped3, commentData.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink3.GetData() == commentData).isTrue();

    //The leading source comment makes the stream JavaScript.
    qor_pp_assert_that(recognizer3.GetDetectedType() == qor::mime::MIMEType::JavaScript).isTrue();
    qor_pp_assert_that(std::string(recognizer3.GetTypeName()) == "application/javascript").isTrue();
}

//Step 12 - the tenth detection increment. A stream that is a plain text document must be classified
//as text/plain: GetDetectedType() == MIMEType::PlainText and GetTypeName() == "text/plain". Plain
//text has no magic number, no markup marker and no container opener, so it is the default for text -
//the whole signature window must be captured and every octet in it must be printable: printable ASCII
//(0x20-0x7E including the space), the three controls the text and JSON conventions allow (tab, line
//feed and carriage return) and the high octets (0x80-0xFF) that carry UTF-8 encoded text. The guard
//is evaluated after every definite signature in the fixed order, so the binary, markup, container and
//source streams recognised in the earlier increments are never reclassified here. The test asserts
//two streams, one per half of the printable range, each with fresh pipeline elements (the established
//shape of the multi-stream cases in steps 5, 9, 10 and 11): stream 1 an ASCII-only prose document and
//stream 2 a document carrying UTF-8 multi-byte octets, written out as explicit octets so the test does
//not depend on the encoding of this source file. As in steps 3-11 the pass-through contract is re-checked
//so that adding a detector cannot break the octet stream proven earlier. This case is written FIRST and
//seen RED - Inspect() has no plain text detector yet.
qor_pp_test_case(mime_recognises_plain_text_signature)
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

    //Stream 1 - an ASCII-only prose document, longer than the sixteen byte signature window. It
    //carries letters, digits, punctuation, spaces, a tab and line feeds - nothing else - so it has
    //no magic number, no markup marker, no container opener and no source file leading signature.
    std::string asciiData;
    asciiData += "Plain prose with letters, digits 1234 and punctuation.\n";
    asciiData += "\tIt has no magic number, no markup marker, no container brace.\n";

    source.SetData(asciiData);

    size_t unitsPumped = 0;
    recognizer.Pump(unitsPumped, asciiData.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink.GetData() == asciiData).isTrue();

    //Every captured octet is printable ASCII, so the stream is plain text.
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::PlainText).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "text/plain").isTrue();

    //Stream 2 - a document carrying UTF-8 encoded text, so the window holds high octets (0x80-0xFF).
    //Fresh elements. The multi-byte sequences are written as explicit octets so the stream does not
    //depend on the encoding of this source file: a two byte sequence (a lead byte followed by one
    //continuation byte) and a three byte sequence (a lead byte followed by two continuation bytes).
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

    std::string utf8Data;
    utf8Data += "Text carrying UTF-8: ";
    utf8Data.push_back(static_cast<char>(0xC3));//two byte lead
    utf8Data.push_back(static_cast<char>(0xA9));//continuation
    utf8Data.push_back(static_cast<char>(0xE2));//three byte lead
    utf8Data.push_back(static_cast<char>(0x82));//continuation
    utf8Data.push_back(static_cast<char>(0xAC));//continuation
    utf8Data += " - and plain ASCII around it.\n";

    source2.SetData(utf8Data);

    size_t unitsPumped2 = 0;
    recognizer2.Pump(unitsPumped2, utf8Data.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink2.GetData() == utf8Data).isTrue();

    //Every captured octet is printable ASCII or a UTF-8 octet, so the stream is plain text.
    qor_pp_assert_that(recognizer2.GetDetectedType() == qor::mime::MIMEType::PlainText).isTrue();
    qor_pp_assert_that(std::string(recognizer2.GetTypeName()) == "text/plain").isTrue();
}

//Step 13 - the fallback. A stream that carries a full signature window and matches none of the ten
//recognised signatures must be classified as application/octet-stream: GetDetectedType() ==
//MIMEType::OctetStream and GetTypeName() == "application/octet-stream". The fallback has no signature
//of its own, so it fires only when every guard above it has declined. Both streams must be longer than
//the sixteen byte signature window so the whole window is captured - a partially captured window is
//not evidence that a stream has no signature, because the octets that carry one may not have arrived
//yet, and such a stream stays Unresolved. Two streams are asserted, each with fresh pipeline elements
//(the established shape of the multi-stream cases in steps 5, 9, 10, 11 and 12). Stream 1 is a raw
//binary blob opening with a NUL: it carries none of the ten signatures and its window holds control
//octets, so the plain text guard rejects it. Stream 2 is the leading octets of a Windows executable
//image - it opens with the printable 'M' 'Z' DOS prefix, so it looks like text at its very start, but
//the padding and offset fields that follow the prefix are NUL octets, so the full window is not
//printable text. As in steps 3-12 the pass-through contract is re-checked so that adding the fallback
//cannot break the octet stream proven earlier. This case is written FIRST and seen RED - Inspect() has
//no fallback yet, so an unmatched stream is still reported as Unresolved.
qor_pp_test_case(mime_falls_back_to_octet_stream)
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

    //Stream 1 - a raw binary blob. The octets are written explicitly so the stream does not depend on
    //the encoding of this source file. It opens with a NUL, so it carries none of the ten signatures,
    //and it is longer than the sixteen byte window so the whole window is captured and holds the
    //control octets that rule the stream out as plain text.
    std::string binaryData;
    binaryData.push_back(static_cast<char>(0x00));//NUL
    binaryData.push_back(static_cast<char>(0x01));//a C0 control
    binaryData.push_back(static_cast<char>(0x02));//another C0 control
    binaryData.push_back(static_cast<char>(0x03));//and another
    binaryData.push_back(static_cast<char>(0x1B));//the escape control
    binaryData.push_back(static_cast<char>(0x7F));//DEL
    binaryData.push_back(static_cast<char>(0x80));//a high octet
    binaryData.push_back(static_cast<char>(0xFF));//the highest octet
    binaryData += " printable payload octets so that the stream is longer than the signature window.";

    source.SetData(binaryData);

    size_t unitsPumped = 0;
    recognizer.Pump(unitsPumped, binaryData.length());

    //The recognizer must not alter the octet stream, control octets included.
    qor_pp_assert_that(sink.GetData() == binaryData).isTrue();

    //The stream is longer than the window, so the whole window was captured and the fallback has
    //enough evidence to classify the stream.
    qor_pp_assert_that(recognizer.GetSignatureByteCount() == 16).isTrue();

    //No signature matched and the window is not printable text, so the stream is an octet stream.
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::OctetStream).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "application/octet-stream").isTrue();

    //Stream 2 - the leading octets of a Windows executable image. Fresh elements. It opens with the
    //printable 'M' 'Z' DOS prefix, so the window starts like text, but the padding byte and the offset
    //fields that follow the prefix are octets a text document cannot carry, so the full window is not
    //printable text and the stream must fall back rather than stay unresolved.
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

    std::string exeData;
    exeData += "MZ";//the printable DOS prefix of an executable image
    exeData.push_back(static_cast<char>(0x90));//the padding byte that follows the prefix
    exeData.push_back(static_cast<char>(0x00));//NUL - the fields after the prefix
    exeData.push_back(static_cast<char>(0x00));
    exeData.push_back(static_cast<char>(0x00));
    exeData.push_back(static_cast<char>(0x00));
    exeData += " payload bytes following the executable header.";

    source2.SetData(exeData);

    size_t unitsPumped2 = 0;
    recognizer2.Pump(unitsPumped2, exeData.length());

    //The recognizer must not alter the octet stream.
    qor_pp_assert_that(sink2.GetData() == exeData).isTrue();

    //The stream is longer than the window, so the whole window was captured.
    qor_pp_assert_that(recognizer2.GetSignatureByteCount() == 16).isTrue();

    //A printable prefix is not evidence of text once the window holds NUL octets, so the stream is an
    //octet stream.
    qor_pp_assert_that(recognizer2.GetDetectedType() == qor::mime::MIMEType::OctetStream).isTrue();
    qor_pp_assert_that(std::string(recognizer2.GetTypeName()) == "application/octet-stream").isTrue();
}

//Step 14 - the chunking increment. Recognition must not depend on how the stream was chunked. The eight
//byte PNG magic is longer than a three byte pump chunk, so it is split across three chunks: three octets,
//three more, then the last two arriving with the payload octets that follow them. The signature window
//accumulates across chunks (m_signatureCount is a member of the filter and Inspect() runs again on every
//chunk), so the window must end full at sixteen octets and the classification must match the single-pump
//PNG case of step 3: GetDetectedType() == MIMEType::PNG and GetTypeName() == "image/png". As in steps 3-13
//the pass-through contract is re-checked so that chunking cannot corrupt the octet stream. This case is
//written FIRST; no implementation change is expected because the window already accumulates across chunks.
qor_pp_test_case(mime_recognises_type_split_across_chunks)
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

    //Stream - the same PNG stream as step 3: the eight byte PNG magic 89 50 4E 47 0D 0A 1A 0A written as
    //explicit octets, then a payload that takes the stream past the sixteen byte signature window.
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

    //Pump the stream three octets at a time. No chunk carries a complete signature: the magic spans three
    //chunks (octets 1-3, 4-6 and 7-8 of it).
    constexpr size_t chunkSize = 3;
    for(size_t streamOffset = 0; streamOffset < pngData.length(); streamOffset += chunkSize)
    {
        size_t chunkUnits = 0;
        size_t bytesLeft = pngData.length() - streamOffset;
        size_t unitsThisChunk = bytesLeft < chunkSize ? bytesLeft : chunkSize;
        recognizer.Pump(chunkUnits, unitsThisChunk);
        //A stalled pump makes no progress; stop rather than spin forever. The asserts below decide.
        if(chunkUnits == 0)
        {
            break;
        }
    }

    //The recognizer must not alter the octet stream, however it was chunked.
    qor_pp_assert_that(sink.GetData() == pngData).isTrue();

    //The stream is longer than the window and every chunk was pumped, so the whole window was captured
    //across the chunks.
    qor_pp_assert_that(recognizer.GetSignatureByteCount() == 16).isTrue();

    //The magic was split across chunks yet the window accumulated it, so the stream is still a PNG.
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::PNG).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "image/png").isTrue();
}



//Step 15 - the reuse increment. One recognizer instance must be able to serve more than one stream.
//After Reset() the detected type and the captured signature must both be discarded: GetDetectedType()
//== MIMEType::Unresolved, GetTypeName() == "" and GetSignatureByteCount() == 0. The stream that
//follows must then be recognised from scratch, not carried over from the stream before it - which is
//why the first stream here is the PNG stream of step 3, so the state being cleared is a KNOWN type
//rather than the initial state, and the second stream is the PDF stream of step 6. Both streams use
//fresh pipeline elements (the established shape of the multi-stream cases in steps 5, 9, 10, 11, 12
//and 13) with the ONE recognizer re-wired to them - the reuse under test is the recognizer, not the
//plumbing. Filter::SetSource links a source's sink to the filter and Filter::SetSink links a sink's
//source to it, so the chain for stream 2 is source2 -> recognizer -> sink2. Pass-through is re-checked
//per stream as in steps 3-14. This case is written FIRST; NO implementation change is expected -
//Reset() is already implemented in mime.cpp (it zeroes m_signatureCount and restores m_detectedType),
//so the case verifies existing behaviour.
qor_pp_test_case(mime_recognizer_resets_between_streams)
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

    //The one recognizer that serves both streams.
    qor::mime::MIMERecognizerFilter recognizer;
    recognizer.SetParent(&testPipeline);
    recognizer.SetSource(&source);
    recognizer.SetSink(&sink);

    //Stream 1 - the PNG stream of step 3: the eight byte PNG magic 89 50 4E 47 0D 0A 1A 0A as
    //explicit octets, then a payload that takes the stream past the sixteen byte signature window.
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

    //The stream is longer than the window, so the whole window is captured and the PNG magic
    //inside it decides the type. This is the state Reset() has to discard.
    qor_pp_assert_that(recognizer.GetSignatureByteCount() == 16).isTrue();
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::PNG).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "image/png").isTrue();

    //Reset must discard the recognised type, its name and every captured octet.
    recognizer.Reset();

    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::Unresolved).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "").isTrue();
    qor_pp_assert_that(recognizer.GetSignatureByteCount() == 0).isTrue();

    //Stream 2 - fresh pipeline elements, and the SAME recognizer re-wired to them.
    ByteBuffer inBuffer2(128);
    ByteBuffer outBuffer2(128);

    StringSource source2;
    source2.SetParent(&testPipeline);
    source2.SetBuffer(&inBuffer2);

    StringSink sink2;
    sink2.SetParent(&testPipeline);
    sink2.SetBuffer(&outBuffer2);

    recognizer.SetSource(&source2);
    recognizer.SetSink(&sink2);

    //The four byte PDF file signature, the version marker, and a body that takes the stream
    //past the sixteen byte window - the same stream as step 6.
    std::string pdfData;
    pdfData += "%PDF-1.4\n";
    pdfData += " payload bytes so that the stream is longer than the signature window.";

    source2.SetData(pdfData);

    size_t unitsPumped2 = 0;
    recognizer.Pump(unitsPumped2, pdfData.length());

    //The recognizer must not alter the octet stream, not even the one it recognises second.
    qor_pp_assert_that(sink2.GetData() == pdfData).isTrue();

    //The window was refilled from scratch after the Reset, and the PDF magic inside it decides
    //the type - the PNG of stream 1 did not carry over.
    qor_pp_assert_that(recognizer.GetSignatureByteCount() == 16).isTrue();
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::PDF).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "application/pdf").isTrue();
}

//Step 16 - two filters in series: the recognizer classifies the stream and a Base64EncodeFilter
//behind it encodes it, so the sink text is the base64 of the stream while the recognizer still
//reports the type of the pre-encoded stream. The encode filter is an InlineFilter<byte> - a pair
//of buffers, not an Element - so the framework inserts it as the buffer shared by the two elements
//either side of it (Pipeline::InsertInlineFilter, BeforeSink position: sink->SetBuffer(filter) and
//the sink's source element's SetBuffer(filter)). Here that is the sink's buffer: the recognizer
//writes raw octets into the filter's upstream side (WriteRequest/WriteAcknowledge run the encode),
//and the sink reads encoded octets out of its downstream side (ReadRequest hands the encoded side).
//InlineFilter::WriteAcknowledge rewrites the item count it is handed to the number of units it
//actually produced - the encoded count - and TransferOctets propagates that count, so the sink is
//asked for the 24 encoded units the 18 raw units produced, not just the 18 pushed units. Octet
//width expectations of the filter: unit size one byte; it pads its output per call, so the stream
//must arrive in whole chunks whose length is a multiple of three; and its default ctor has zero
//items, so it is built here with an explicit capacity. The stream is the PNG magic as explicit
//octets plus a ten byte payload - eighteen octets, one 3-aligned chunk, longer than the 16 byte
//signature window - pumped for the 24 encoded units. The expected base64 text is the standard
//base64 of the stream (computed independently, no padding since 18 is a multiple of 3).
qor_pp_test_case(mime_recogniser_chained_with_qor_base64encodefilter)
{
    //Push-mode wiring is the working pump path for a filter (see step 1 notes).
    Pipeline testPipeline;
    testPipeline.SetFlowMode(Element::FlowMode::Push);

    ByteBuffer inBuffer(128);

    StringSource source;
    source.SetParent(&testPipeline);
    source.SetBuffer(&inBuffer);

    StringSink sink;
    sink.SetParent(&testPipeline);

    //The encode filter is the buffer of the sink - the InsertInlineFilter BeforeSink position
    //between the recognizer and the sink. Built with an explicit capacity; its default ctor
    //would have zero items and encode nothing.
    Base64EncodeFilter encodeFilter(128);
    sink.SetBuffer(&encodeFilter);

    qor::mime::MIMERecognizerFilter recognizer;
    recognizer.SetParent(&testPipeline);
    recognizer.SetSource(&source);
    recognizer.SetSink(&sink);

    //The eight byte PNG magic as explicit octets, then a ten byte payload, so the whole stream is
    //18 octets - a multiple of three, one pumped chunk, and longer than the signature window.
    std::string pngData;
    pngData.push_back(static_cast<char>(0x89));
    pngData.push_back(static_cast<char>(0x50));
    pngData.push_back(static_cast<char>(0x4E));
    pngData.push_back(static_cast<char>(0x47));
    pngData.push_back(static_cast<char>(0x0D));
    pngData.push_back(static_cast<char>(0x0A));
    pngData.push_back(static_cast<char>(0x1A));
    pngData.push_back(static_cast<char>(0x0A));
    pngData += "IHDR qor01";

    source.SetData(pngData);

    //Pump for the encoded length: the pump counts units as the sink sees them, and the encode
    //filter reports 4 encoded units per 3 raw units through InlineFilter::WriteAcknowledge.
    size_t unitsPumped = 0;
    recognizer.Pump(unitsPumped, 24);

    //The sink text must be exactly the base64 of the stream.
    qor_pp_assert_that(sink.GetData() == "iVBORw0KGgpJSERSIHFvcjAx").isTrue();

    //The recognizer inspected the raw octets before they were encoded downstream, so it still
    //classifies the stream by its magic.
    qor_pp_assert_that(recognizer.GetSignatureByteCount() == 16).isTrue();
    qor_pp_assert_that(recognizer.GetDetectedType() == qor::mime::MIMEType::PNG).isTrue();
    qor_pp_assert_that(std::string(recognizer.GetTypeName()) == "image/png").isTrue();
}

//application-level glue: the recognised MIME type name selects one of the candidate sinks supplied by the caller.
static StringSink* SelectSinkByMIMETypeName(const std::string& typeName, StringSink& imageSink, StringSink& textSink)
{
    if(typeName == "image/png")
        return &imageSink;
    if(typeName == "text/plain")
        return &textSink;
    return nullptr;
}

qor_pp_test_case(mime_type_name_drives_a_sink)
{
    //First recognise the PNG stream and capture the registered name.
    Pipeline pngPipeline;
    pngPipeline.SetFlowMode(Element::FlowMode::Push);
    ByteBuffer pngBuffer(128);
    StringSource pngSource;
    pngSource.SetParent(&pngPipeline);
    pngSource.SetBuffer(&pngBuffer);
    StringSink pngSink;
    pngSink.SetParent(&pngPipeline);
    pngSink.SetBuffer(&pngBuffer);

    qor::mime::MIMERecognizerFilter pngRecognizer;
    pngRecognizer.SetParent(&pngPipeline);
    pngRecognizer.SetSource(&pngSource);
    pngRecognizer.SetSink(&pngSink);

    std::string pngData;
    pngData.push_back(static_cast<char>(0x89));
    pngData.push_back(static_cast<char>(0x50));
    pngData.push_back(static_cast<char>(0x4E));
    pngData.push_back(static_cast<char>(0x47));
    pngData.push_back(static_cast<char>(0x0D));
    pngData.push_back(static_cast<char>(0x0A));
    pngData.push_back(static_cast<char>(0x1A));
    pngData.push_back(static_cast<char>(0x0A));
    pngData += "IHDR qor01";

    pngSource.SetData(pngData);
    size_t pngUnitsPumped = 0;
    pngRecognizer.Pump(pngUnitsPumped, pngData.length());

    qor_pp_assert_that(pngSink.GetData() == pngData).isTrue();
    qor_pp_assert_that(pngRecognizer.GetSignatureByteCount() == 16).isTrue();
    qor_pp_assert_that(pngRecognizer.GetDetectedType() == qor::mime::MIMEType::PNG).isTrue();

    const std::string pngTypeName = pngRecognizer.GetTypeName();
    qor_pp_assert_that(pngTypeName == "image/png").isTrue();

    //The routing area has two candidate sinks, but the stream is routed only to the one selected by the name.
    Pipeline routePipeline;
    routePipeline.SetFlowMode(Element::FlowMode::Push);
    ByteBuffer routeBuffer(128);
    ByteBuffer imageBuffer(128);
    ByteBuffer textBuffer(128);

    StringSink routeImageSink;
    routeImageSink.SetParent(&routePipeline);
    routeImageSink.SetBuffer(&imageBuffer);

    StringSink routeTextSink;
    routeTextSink.SetParent(&routePipeline);
    routeTextSink.SetBuffer(&textBuffer);

    StringSink* selectedSink = SelectSinkByMIMETypeName(pngTypeName, routeImageSink, routeTextSink);
    qor_pp_assert_that(selectedSink != nullptr).isTrue();
    qor_pp_assert_that(selectedSink == &routeImageSink).isTrue();

    StringSource pngRouteSource;
    pngRouteSource.SetParent(&routePipeline);
    pngRouteSource.SetBuffer(&routeBuffer);
    pngRouteSource.SetData(pngData);

    qor::mime::MIMERecognizerFilter pngRouteRecognizer;
    pngRouteRecognizer.SetParent(&routePipeline);
    pngRouteRecognizer.SetSource(&pngRouteSource);
    pngRouteRecognizer.SetSink(selectedSink);

    size_t pngRouteUnitsPumped = 0;
    pngRouteRecognizer.Pump(pngRouteUnitsPumped, pngData.length());

    qor_pp_assert_that(routeImageSink.GetData() == pngData).isTrue();
    qor_pp_assert_that(routeTextSink.GetData() == std::string()).isTrue();

    //A plain text stream must select the text sink instead.
    std::string textData = "The quick brown fox jumps over the lazy dog";

    Pipeline textPipeline;
    textPipeline.SetFlowMode(Element::FlowMode::Push);
    ByteBuffer textBuffer2(128);
    StringSource textSource;
    textSource.SetParent(&textPipeline);
    textSource.SetBuffer(&textBuffer2);
    StringSink textSink;
    textSink.SetParent(&textPipeline);
    textSink.SetBuffer(&textBuffer2);

    qor::mime::MIMERecognizerFilter textRecognizer;
    textRecognizer.SetParent(&textPipeline);
    textRecognizer.SetSource(&textSource);
    textRecognizer.SetSink(&textSink);

    textSource.SetData(textData);
    size_t textUnitsPumped = 0;
    textRecognizer.Pump(textUnitsPumped, textData.length());

    qor_pp_assert_that(textSink.GetData() == textData).isTrue();
    qor_pp_assert_that(textRecognizer.GetSignatureByteCount() == 16).isTrue();
    qor_pp_assert_that(textRecognizer.GetDetectedType() == qor::mime::MIMEType::PlainText).isTrue();

    const std::string textTypeName = textRecognizer.GetTypeName();
    qor_pp_assert_that(textTypeName == "text/plain").isTrue();

    selectedSink = SelectSinkByMIMETypeName(textTypeName, routeImageSink, routeTextSink);
    qor_pp_assert_that(selectedSink == &routeTextSink).isTrue();

    StringSource textRouteSource;
    textRouteSource.SetParent(&routePipeline);
    textRouteSource.SetBuffer(&routeBuffer);
    textRouteSource.SetData(textData);

    qor::mime::MIMERecognizerFilter textRouteRecognizer;
    textRouteRecognizer.SetParent(&routePipeline);
    textRouteRecognizer.SetSource(&textRouteSource);
    textRouteRecognizer.SetSink(selectedSink);

    size_t textRouteUnitsPumped = 0;
    textRouteRecognizer.Pump(textRouteUnitsPumped, textData.length());

    qor_pp_assert_that(routeTextSink.GetData() == textData).isTrue();
    qor_pp_assert_that(routeImageSink.GetData() == pngData).isTrue();
}
