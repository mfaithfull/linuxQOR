// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <filesystem>
#include "fileindex.h"

namespace qor{ namespace io{ namespace filesystem {

    Index::Index() : m_dirent() {}
    
    Index::Index(const Path & path, const std::string& fileName) : m_dirent(std::filesystem::path(path.ToString(fileName))){}

    Index::Index(const std::filesystem::directory_entry& dirent) : m_dirent(dirent) {}

    Index::~Index() = default;

    Index::Index(const Index& src)
    {
        *this = src;
    }

    Index& Index::operator = (const Index& src)
    {
        if(&src != this)
        {
            m_dirent = src.m_dirent;
        }
        return *this;
    }

    void Index::Set(const Path & path, const std::string& fileName)
    {
        std::filesystem::directory_entry dirent(std::filesystem::path(path.ToString(fileName)));
        m_dirent = dirent;
    }
    
    bool Index::Exists(void) const
    {
        return m_dirent.exists();
    }

    bool Index::Copy(const Index & Destination, std::filesystem::copy_options copyOptions) const
    {
        return std::filesystem::copy_file(m_dirent.path(), Destination.ToString(), copyOptions);
    }

    bool Index::Delete() const
    {
        return std::filesystem::remove(m_dirent.path());
    }

    bool Index::Move(const Index & Destination) const
    {
        std::filesystem::rename(m_dirent.path(), Destination.ToString());
        return true;
    }

    bool Index::Rename(const Index & Destination)
    {
        m_dirent.replace_filename(Destination.ToString());
        return true;
    }

    bool Index::Resize( std::uintmax_t newSize )
    {
        std::filesystem::resize_file(m_dirent.path(), newSize);
        return true;
    }

    bool Index::IsBlockFile() const
    {
        return std::filesystem::is_block_file(m_dirent.path());
    }

    bool Index::IsCharacterFile() const
    {
        return std::filesystem::is_character_file(m_dirent.path());
    }

    bool Index::IsDirectory() const
    {
        return std::filesystem::is_directory(m_dirent.path());
    }

    bool Index::IsFIFO() const
    {
        return std::filesystem::is_fifo(m_dirent.path());
    }

    bool Index::IsOther() const
    {
        return std::filesystem::is_other(m_dirent.path());
    }

    bool Index::IsRegularFile() const
    {
        return std::filesystem::is_regular_file(m_dirent.path());
    }

    bool Index::IsSocket() const
    {
        return std::filesystem::is_socket(m_dirent.path());
    }

    bool Index::IsSymLink() const
    {
        return std::filesystem::is_symlink(m_dirent.path());
    }

    std::uintmax_t Index::Size() const
    {
        return std::filesystem::file_size(m_dirent.path());
    }
    
    std::filesystem::file_time_type Index::LastWriteTime() const
    {
        return m_dirent.last_write_time();
    }

    std::filesystem::file_status Index::Status() const
    {
        return m_dirent.status();
    }

    std::filesystem::file_status Index::SymLinkStatus() const
    {
        return m_dirent.symlink_status();
    }

    std::string Index::ToString() const
    {        
        return m_dirent.path().generic_string();
    }

    filesystem::Path Index::GetPath() const
    {
        return Path(m_dirent.path().generic_string());
    }
    
    //ref_of<File>::type Create(const int /*openFor*/, const int /*withFlags*/, const int /*inMode*/)
    //{
    //    ref_of<File>::type result;
    //    return result;
    //}

    //ref_of<File>::type Index::Open(const int /*openFor*/, const int /*withFlags*/, const int /*inMode*/)    
    //{
    //    ref_of<File>::type result;
    //    return result;
    //}
    
}}}//qor::io::filesystem
