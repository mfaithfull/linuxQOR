// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MIME_MIME
#define QOR_PP_H_MIME_MIME

#include <cstddef>
#include "src/framework/data/pipeline/filter.h"

//A MIME type recognizer expressed as a data pipeline filter. The filter sits between a
//source and a sink in a pipeline. It never alters the octet stream; it inspects the
//leading octets of whatever passes through it and classifies the stream as one of the
//ten popular MIME types (see purpose.md and memory-bank/qor_mime_plan.md). The
//recognised type is read back by the application or by the elements downstream.

namespace qor{ namespace mime{

    //The ten popular MIME types the recognizer targets, the fallback type used when no
    //signature matches, and the state before any octet has been inspected.
    enum class MIMEType
    {
        Unresolved,        //No octets inspected yet
        OctetStream,       //application/octet-stream - fallback, not one of the ten
        PlainText,         //text/plain
        HTML,              //text/html
        JSON,              //application/json
        XML,               //application/xml
        JavaScript,        //application/javascript
        PNG,               //image/png
        JPEG,              //image/jpeg
        GIF,               //image/gif
        PDF,               //application/pdf
        ZIP,               //application/zip
    };

    class qor_pp_module_interface_gcc(QOR_MIME) MIMERecognizerFilter : public qor::pipeline::Filter
    {
    public:

        qor_pp_module_interface(QOR_MIME) MIMERecognizerFilter();
        qor_pp_module_interface(QOR_MIME) virtual ~MIMERecognizerFilter();
        qor_pp_module_interface(QOR_MIME) MIMERecognizerFilter& operator = (const MIMERecognizerFilter& src);

        //Pipeline filter hooks. WriteFilter is used in pull mode, ReadFilter in push mode.
        //Both must be present so the element works whichever way the pipeline is pumped.
        qor_pp_module_interface(QOR_MIME) virtual bool ReadFilter(size_t& unitsProcessed, size_t unitsToProcess) override;
        qor_pp_module_interface(QOR_MIME) virtual bool WriteFilter(size_t& unitsProcessed, size_t unitsToProcess) override;

        //The type recognised so far for the stream that has passed through this filter.
        qor_pp_module_interface(QOR_MIME) virtual MIMEType GetDetectedType() const;
        //The recognised type as its registered name, e.g. "image/png". Null terminated,
        //empty string when Unresolved.
        qor_pp_module_interface(QOR_MIME) virtual const char* GetTypeName() const;
        //How many leading octets have been captured into the signature window so far.
        qor_pp_module_interface(QOR_MIME) virtual size_t GetSignatureByteCount() const;
        //Discard the captured signature and the detected type so that the same filter
        //instance can be used for another stream.
        qor_pp_module_interface(QOR_MIME) void Reset(void);

    protected:

        //Capture the octets that are currently available into the signature window and
        //update the candidate type. Does not consume or move the octets.
        qor_pp_module_interface(QOR_MIME) virtual bool Inspect(size_t& unitsInspected, size_t unitsToInspect);

    protected:

        //The largest signature any of the recognised types needs; PNG needs its eight
        //byte header and the text types need a markup sample, so sixteen bytes is enough.
        static constexpr size_t SIGNATURE_WINDOW_BYTES = 16;

        qor::byte m_signature[SIGNATURE_WINDOW_BYTES];
        size_t m_signatureCount{0};
        MIMEType m_detectedType{MIMEType::Unresolved};
    };

}}//qor::mime

#endif//QOR_PP_H_MIME_MIME
