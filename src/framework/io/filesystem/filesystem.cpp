// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "filesystem.h"
#include "path.h"

namespace qor{ namespace io{

    FileSystem::FileSystem()
    {
        m_pimpl = new_ref<IFileSystem>();
    }

    FileSystem::~FileSystem() noexcept = default;

    void FileSystem::Setup()
    {
        if(m_pimpl.IsNotNull())
        {
            filesystem::Path::s_separator = m_pimpl->PathSeparator();
            filesystem::Path::s_selfIndicator = m_pimpl->SelfIndicator();
            filesystem::Path::s_parentIndicator = m_pimpl->ParentIndicator();
            filesystem::Path::s_maxElementLength = m_pimpl->MaxElementLength();
            filesystem::Path::s_rootIndicator = m_pimpl->RootIndicator();
        }
        m_root.Setup();
    }

    void FileSystem::Shutdown(){}

    const filesystem::Root& FileSystem::GetRoot() const
    {
        return m_root;
    }

    filesystem::Path FileSystem::CurrentPath() const
    {
        std::filesystem::path stdpath = std::filesystem::current_path();
        filesystem::Path currentPath(stdpath.string());
        return currentPath;
    }

    void FileSystem::CurrentPath(filesystem::Path& path) const
    {
        std::filesystem::current_path(path);
    }

    ref_of<File>::type FileSystem::Create(const filesystem::Index& index, const int withFlags) const
    {
        return m_pimpl->Create(index, withFlags);
    }

    std::optional<filesystem::Folder> FileSystem::Create(const filesystem::Path& path) const
    {
        return MakeDir(path);
    }

    std::optional<filesystem::Folder> FileSystem::MakeDir(const filesystem::Path& path) const
    {
        std::optional<filesystem::Folder> folder;
        try{
            if(std::filesystem::create_directory(path))
            {
                folder.emplace(filesystem::Folder(path));                
            }
        }
        catch(std::filesystem::filesystem_error& fse)
        {
            continuable(fse.what());
        }
        return folder;
    }

    std::optional<filesystem::Folder> FileSystem::NewFolder(const filesystem::Path& path) const
    {
        return MakeDir(path);
    }


    bool FileSystem::Delete(const filesystem::Index& index) const
    {
        try{
            return std::filesystem::remove(index.GetPath());
        }
        catch(std::filesystem::filesystem_error& fse)
        {
            continuable(fse.what());
        }
        return false;
    }

    bool FileSystem::RemoveDir(const filesystem::Path& path) const
    {
        try
        {
            return std::filesystem::remove(path);
        }
        catch(std::filesystem::filesystem_error& fse)
        {
            continuable(fse.what());
        }
        return false;            
    }
    
    bool FileSystem::DeleteFolder(const filesystem::Path& path) const
    {
        return RemoveDir(path);
    }

    ref_of<File>::type FileSystem::Open(const filesystem::Index& index, const int openFor, const int withFlags) const
    {
        return m_pimpl->Open(index, openFor, withFlags);
    }

    bool FileSystem::Copy(const filesystem::Index& srcIndex, const filesystem::Index& destIndex) const
    {
        try
        {
            std::filesystem::copy(srcIndex.GetPath(), destIndex.GetPath());
            return true;
        }
        catch(std::filesystem::filesystem_error& fse)
        {
            continuable(fse.what());
        }

        return false;
    }

    bool FileSystem::Move(const filesystem::Index& srcIndex, const filesystem::Index& destIndex) const
    {
        return srcIndex.Move(destIndex);
    }

    bool FileSystem::Rename(filesystem::Index& srcIndex, const filesystem::Index& destIndex) const
    {
        {
            try
            {
                std::filesystem::rename(srcIndex.GetPath(), destIndex.GetPath());
                return true;
            }
            catch(std::filesystem::filesystem_error& fse)
            {
                continuable(fse.what());
            }
            return false;
        }
    }

    std::filesystem::space_info FileSystem::Space(const filesystem::Path& path) const
    {
        return std::filesystem::space(path);
    }

    filesystem::Path FileSystem::TempFolder() const
    {
        filesystem::Path tempPath(std::filesystem::temp_directory_path().generic_string());
        return tempPath;
    }

    filesystem::Path FileSystem::ApplicationLogPath() const
    {
        return m_pimpl->ApplicationLogPath();
    }

}}//qor::io
