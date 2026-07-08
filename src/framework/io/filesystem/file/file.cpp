// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "file.h"

namespace qor{ namespace io{

    ref_of<File>::type File::Open(const filesystem::Index& index, int openFor, int withFlags)
    {
        return new_ref<File>(index, openFor, withFlags);
    }

    //Base implementation of File

    File::File(){ }

    File::File(int fd) : File()
    {
        Descriptor::m_fd = fd;
    }

    File::File(const Descriptor& descriptor) : Descriptor(descriptor){ }

    File::File(const File& src) : m_index(src.m_index)
    {
        *this = src;
    }

    File::File(const filesystem::Index& index) : m_index(index){ }

    File::File(const filesystem::Index& index, int /*OpenFor*/, const int /*WithFlags*/) : m_index(index){ }

    File& File::operator = (const File& src)
    {
        if(&src != this)
        {
            m_index = src.m_index;
        }
        return *this;
    }

    File::~File(){/*derived class owns platform specific resource and handles close in its destructor*/}

    uint64_t File::GetSize()
    {
        return std::filesystem::file_size(m_index.GetPath());
    }

    uintmax_t File::GetHardLinkCount()
    {
        return std::filesystem::hard_link_count(m_index.GetPath());
    }

    FileTime File::GetLastWriteTime()
    {
        return std::filesystem::last_write_time(m_index.GetPath());
    }

    filesystem::Permissions File::GetPermissions()
    {
        return static_cast<filesystem::Permissions>(std::filesystem::status(m_index.GetPath()).permissions());
    }

    void File::SetPermissions(const filesystem::Permissions permissions, const filesystem::PermissionOptions options)
    {
        try
        {
            std::filesystem::permissions(m_index.GetPath(),
                static_cast<std::filesystem::perms>(permissions),
                static_cast<std::filesystem::perm_options>(options));
        }
        catch(std::filesystem::filesystem_error& fse)
        {
            continuable(fse.what());
        }
    }

    void File::ReSize(uintmax_t size)
    {
        std::filesystem::resize_file(m_index.GetPath(), size);
    }

    FileStatus File::GetStatus()
    {
        try
        {
            return std::filesystem::status(m_index.GetPath());
        }
        catch(std::filesystem::filesystem_error& fse)
        {
            continuable(fse.what());
        }
        return FileStatus();
    }

    FileStatus File::GetSymLinkStatus()
    {
        try
        {
            return std::filesystem::symlink_status(m_index.GetPath());
        }
        catch(std::filesystem::filesystem_error& fse)
        {
            continuable(fse.what());
        }
        return FileStatus();
    }

    filesystem::Type File::GetType()
    {
        return static_cast<filesystem::Type>(std::filesystem::status(m_index.GetPath()).type());
    }

    //Empty base implementations
    //Don't call these, override them per OS
    bool File::SupportsPosition()
    {
        serious("Empty base implementation.");
        return false;
    }

    uint64_t File::GetPosition()
    {
        serious("Empty base implementation.");
        return 0;
    }

    long File::SetPosition(long /*offset*/, int /*whence*/)
    {
        serious("Empty base implementation.");
        return 0;
    }

    uint64_t File::SetPosition(uint64_t /*newPosition*/)
    {
        serious("Empty base implementation.");
        return 0;
    }

    uint64_t File::SetPositionRelative(int64_t /*offset*/)
    {
        serious("Empty base implementation.");
        return 0;
    }

    void File::Truncate(uint64_t /*length*/)
    {
        serious("Empty base implementation.");
    }

    void File::Reserve(uint64_t /*length*/)
    {
        serious("Empty base implementation.");
    }

    void File::Flush()
    {
        serious("Empty base implementation.");
    }

    ref_of<File>::type File::ReOpen(int /*openFor*/, int /*withFlags*/)
    {
        serious("Empty base implementation.");
        ref_of<File>::type newfile;
        return newfile;
    }

    task<int> File::AsyncRead(const qor::io::async::Interface& /*ioContext*/, byte* /*buffer*/, size_t /*byteCount*/, off_t /*offset*/)
    {
        serious("Empty base implementation.");
        return task<int>{};
    }

    task<int> File::AsyncWrite(const qor::io::async::Interface& /*ioContext*/, byte* /*buffer*/, size_t /*byteCount*/, off_t /*offset*/)
    {
        serious("Empty base implementation.");
        return task<int>{};
    }

    int64_t File::Read(byte* /*buffer*/, size_t /*byteCount*/, int64_t /*offset*/)
    {
        serious("Empty base implementation.");
        return 0;
    }

    int64_t File::Write(byte* /*buffer*/, size_t /*byteCount*/, int64_t /*offset*/)
    {
        serious("Empty base implementation.");
        return 0;
    }

}}//qor::io
