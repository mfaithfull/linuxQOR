// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "iosource.h"
#include "sink.h"
#include "src/qor/flyers/error/error.h"

namespace qor{ namespace pipeline{

    iosource_base::iosource_base() : pipeline::Source(), m_plug(nullptr) { }

    iosource_base::~iosource_base() = default;

    void iosource_base::SetPlug(Element* plug)
    {
        m_plug = plug;
    }

    pipeline::Element* iosource_base::GetPlug() const
    {
        return m_plug;
    }

    bool iosource_base::HasPlug()
    {
        return m_plug != nullptr;
    }

    pipeline::Plug* iosource_base::ActualPlug()
    {
        return dynamic_cast<pipeline::Plug*>(m_plug);
    }

    const char* iosource_base::Name() const
    {
        return "iosource_base";
    }

    size_t iosource_base::ReadBytes(byte* /*space*/, size_t /*bytesToRead*/)
    {
        fatal("Empty base called. Please overrride bool ReadBytes(byte*, size_t); in your pipeline::io_source derived class.");
        return 0;
    }

}}//qor::components
