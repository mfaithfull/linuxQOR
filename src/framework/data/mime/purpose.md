# purpose.md — src/framework/data/mime — qor_mime

MIME type recognition as a data pipeline filter.

A pipeline is a chain source -> (filter)* -> sink and data always flows source to sink.
A filter is both a Sink and a Source (see src/framework/data/pipeline/purpose.md). Most
filters transform the octets that pass through them (base64 encode/decode, unicode text).
This one does not. It recognises what the octets are.

    source --> [ MIMERecognizerFilter ] --> sink
                    |
                    +-- captures the leading octets into a signature window
                    +-- classifies the stream as one of the ten popular MIME types
                    +-- leaves the stream untouched, so the sink sees the original data

The recognised type is read back from the filter (GetDetectedType()/GetTypeName()), so an
application or a downstream element can act on it without the stream being diverted. This
is the missing half of the QOR for Content Type work: the HTTP request generator has a
TODO about MIME in src/components/protocols/http, the UI renderer loader converts a mime
type string to a file type by hand, and qor::framework::Resource::Type() is documented as
"maybe mime-type of the resource".

Ten popular MIME types targeted, plus application/octet-stream as the fallback:

    text/plain        text/html       application/json      application/xml
    application/javascript            image/png             image/jpeg
    image/gif         application/pdf application/zip       (fallback application/octet-stream)

Library: qor_mime (SHARED, QOR_MIME define)
Namespace: qor::mime (framework/data levels are elided, as for qor::pipeline and qor::dio)
Development: test first, one increment at a time. See status.md and
docs/archive/qor_mime_plan.md (the plan, archived out of the memory bank).
