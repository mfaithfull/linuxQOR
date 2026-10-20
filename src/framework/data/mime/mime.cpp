// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "mime.h"

namespace qor{ namespace mime{

    //On MSVC a shared QOR module has to export at least one symbol or no import lib
    //(.lib) is generated and a test exe that links qor_mime fails with LNK1104.
    //This is the future public API entry point of the module.
    qor_pp_export void init_mime(void)
    {
        //placeholder for future qor_mime initialisation
    }

    MIMERecognizerFilter::MIMERecognizerFilter() = default;

    MIMERecognizerFilter::~MIMERecognizerFilter() = default;

    MIMERecognizerFilter& MIMERecognizerFilter::operator = (const MIMERecognizerFilter& src) = default;

    bool MIMERecognizerFilter::ReadFilter(size_t& unitsProcessed, size_t unitsToProcess)
    {
        return Inspect(unitsProcessed, unitsToProcess);
    }

    bool MIMERecognizerFilter::WriteFilter(size_t& unitsProcessed, size_t unitsToProcess)
    {
        return Inspect(unitsProcessed, unitsToProcess);
    }

    //Outline placeholder. The recognizer must not disturb the stream, so this hook
    //keeps the base Filter behaviour of handing the requested units straight to the
    //sink. Buffer based signature capture and type detection are added test first,
    //one increment at a time, see memory-bank/qor_mime_plan.md.
    bool MIMERecognizerFilter::Inspect(size_t& unitsInspected, size_t unitsToInspect)
    {
        unitsInspected = unitsToInspect;
        return true;
    }

    MIMEType MIMERecognizerFilter::GetDetectedType() const
    {
        return m_detectedType;
    }

    const char* MIMERecognizerFilter::GetTypeName() const
    {
        //name table for the recognised types arrives with the type detection increments
        return "";
    }

    size_t MIMERecognizerFilter::GetSignatureByteCount() const
    {
        return m_signatureCount;
    }

    void MIMERecognizerFilter::Reset(void)
    {
        m_signatureCount = 0;
        m_detectedType = MIMEType::Unresolved;
    }

}}//qor::mime
