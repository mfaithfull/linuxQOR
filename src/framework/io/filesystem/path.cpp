// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "path.h"

namespace qor{ namespace io{ namespace filesystem {

    std::string Path::s_rootIndicator;
    std::string Path::s_selfIndicator;
    std::string Path::s_parentIndicator;
    unsigned short Path::s_maxElementLength;
    std::string Path::s_separator;

    Path::Path() : m_path(){ }

    Path::Path(const Path& src) : m_path(src.m_path){ }

    Path::Path(const std::string& path) : m_path(path){ }
    
    Path::~Path() = default;

    Path& Path::operator = (const Path& src)
    {
        if(&src != this)
        {
            m_path = src.m_path;
        }
        return *this;
    }

    Path& Path::operator /= (const std::string& folder)
    {
        m_path /= folder;
        return *this;
    }

    Path Path::operator / (const std::string& folder) const
    {
        Path newPath(*this);
        return newPath /= folder;
    }

    Path Path::Parent() const
    {
        Path parent(m_path.parent_path().generic_string());
        return parent;
    }

    std::string Path::Separator() const
    {
        return Path::s_separator;
    }

    std::string Path::SelfIndicator() const
    {
        return Path::s_selfIndicator;
    }

    std::string Path::ParentIndicator() const
    {
        return Path::s_parentIndicator;
    }

    unsigned short Path::MaxElementLength() const
    {
        return Path::s_maxElementLength;
    }

    std::string Path::RootIndicator() const
    {
        return Path::s_rootIndicator;
    }

    std::string Path::ToString() const
    {
        return m_path.string();
    }

    std::string Path::ToString(std::string FileName) const
    {
        Path child = *this;
        child /= FileName;        
        return child.ToString();
    }

    void Path::Clear()
    {
        m_path.clear();
    }

    Path Path::RootName() const
    {
        Path root(m_path.root_name().generic_string());
        return root;
    }

    Path Path::RootDirectory() const
    {
        Path dir(m_path.root_directory().generic_string());
        return dir;
    }

    Path Path::RelativePath() const
    {
        Path relative(m_path.relative_path().generic_string());
        return relative;
    }

    bool Path::IsEmpty() const
    {
        return m_path.empty();
    }

    bool Path::HasRootPath() const
    {
        return m_path.has_root_path();
    }

    bool Path::HasRootName() const
    {
        return m_path.has_root_name();
    }

    bool Path::HasRootDirectory() const
    {
        return m_path.has_root_directory();
    }

    bool Path::HasRelativePath() const
    {
        return m_path.has_relative_path();
    }

    bool Path::HasParentPath() const
    {
        return m_path.has_parent_path();
    }

    bool Path::IsAbsolute() const
    {
        return m_path.is_absolute();
    }

    bool Path::IsRelative() const
    {
        return m_path.is_relative();
    }

    bool Path::IsSamePath(const Path& compare) const
    {
        return m_path.compare(compare.m_path) == 0 ? true : false;
    }

    Path::operator std::filesystem::path()
    {
        return m_path;
    }

    Path::operator const std::filesystem::path() const
    {
        return m_path;
    }

}}}//qor::io::filesystem
