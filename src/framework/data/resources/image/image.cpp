// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "image.h"

namespace qor{ namespace framework{ 

    void Image::Free()
    {
        if(m_bytes)
        {
            delete [] m_bytes;
            m_bytes = nullptr;
        }
    }

    void Image::SetSize(size_t width, size_t height)
    {
        if((width != m_width) || (height != m_height))
        {
            Free();
            m_bytes = new byte[m_pixelSize * m_width * m_height];
        }
    }

    byte* Image::operator[](size_t index)
    {
        return m_bytes ? m_bytes + (index * m_pixelSize) : nullptr;
    }

    byte* Image::At(size_t x, size_t y)
    {
        return m_bytes ? m_bytes + ((y * m_width * m_pixelSize) + (x * m_pixelSize)) : nullptr;
    }

}}//qor::framework
