// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <filesystem>
#include "folder.h"
#include "src/qor/flyers/error/error.h"

namespace qor{ namespace io{ namespace filesystem {

    Folder::Folder(const Folder& src)
    {
        *this = src;
    }

    Folder::Folder(const class Path& path) : m_path(path) {}

    Folder::~Folder() = default;

    Folder& Folder::operator = (const Folder& src)
    {
        if(&src != this)
        {
            m_path = src.m_path;
        }   
        return *this;     
    }

    bool Folder::Exists()
    {
        return std::filesystem::exists(m_path);
    }

    Folder Folder::Create(class Path& newFolder)
    {
        std::filesystem::create_directory(newFolder);
        return Folder(newFolder);
    }

    bool Folder::Copy( class Path& destParent )
    {
        return std::filesystem::copy_file(m_path.operator std::filesystem::path(), destParent);
    }

    bool Folder::Move(class Path& destParent)
    {
        try
        {
            std::filesystem::rename(m_path.operator const std::filesystem::path(), destParent.operator const std::filesystem::path());
            return true;
        }
        catch(std::filesystem::filesystem_error& fse)
        {
            continuable(fse.what());
        }
        return false;
    }

    bool Folder::Rename(const string_t& name)
    {
        try
        {
            std::filesystem::rename(m_path, m_path.Parent() / name);
            return true;
        }
        catch(std::filesystem::filesystem_error& fse)
        {
            continuable(fse.what());
        }
        return false;
    }

    bool Folder::Delete()
    {
        std::filesystem::remove_all(m_path);
        return std::filesystem::remove(m_path);
    }

    void Folder::Enumerate( const std::function <bool (Index&)>& f ) const
    {
        for (auto const& dir_entry : std::filesystem::directory_iterator{m_path}) 
        {
            Index item(dir_entry);
            if( !f(item) )
            {
                break;
            }
        }
    }

    Permissions Folder::GetPermissions()
    {
        Permissions perms = static_cast<Permissions>(std::filesystem::status(m_path).permissions());
        return perms;
    }

    void Folder::SetPermissions(const Permissions& permissions, const PermissionOptions& options)
    {
        try
        {
            std::filesystem::permissions(m_path, 
                static_cast<std::filesystem::perms>(permissions), 
                static_cast<std::filesystem::perm_options>(options));        
        }
        catch(std::filesystem::filesystem_error& fse)
        {
            continuable(fse.what());
        }
    }

    void Folder::CreateSymLinkTo(class Path& target)
    {
        std::filesystem::create_symlink(target, m_path);
    }

    class Path Folder::Path()
    {
        return m_path;
    }

}}}//qor::io::filesystem
