// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "what.h"

namespace qor{ 

    What::What(const std::string& what) : m_content(what){ }

    What::What(const What& src)
    {
        m_content = src.m_content;
    }

    What::~What() noexcept { }

    What& What::operator = (const What& src)
    {
        if(&src != this)
        {
            m_content = src.m_content;
        }
        return *this;
    }

    const std::string& What::Content() const
    {
        return m_content;
    }

}//qor
