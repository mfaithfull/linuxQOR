// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "root.h"

namespace qor{ namespace io{ namespace filesystem{

    Root::Root() : m_path(){ }

    Root::Root(const Root& src) : m_path(src.m_path){ }

    Root::~Root() = default;

    void Root::Setup()
    {
        m_path = Path().RootIndicator();
    }
    
    Root& Root::operator = (const Root& src)
    {
        if(&src != this)
        {
            m_path = src.m_path;
        }
        return *this;
    }
    
    class Path Root::operator / (const std::string folder) const
    {
        class Path path = m_path;
        path /= folder;
        return path;
    }

    class Path Root::Path() const
    {
        return m_path;
    }

    class Folder Root::Folder() const
    {
        class Folder result(m_path);
        return result;
    }

    const std::string Root::Indicator() const
    {
        return m_path.RootIndicator();
    }

}}}//qor::io::filesystem
