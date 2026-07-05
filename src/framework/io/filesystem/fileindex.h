// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_FILESYSTEM_FILEINDEX
#define QOR_PP_H_PLATFORM_FILESYSTEM_FILEINDEX

#include <string>
#include <filesystem>
#include "path.h"
#include "src/qor/memory/reference/reference.h"

namespace qor{ namespace io{ namespace filesystem {

    class qor_pp_module_interface(QOR_FILESYSTEM) Index
	{
	public:

        Index();
        Index(const Path& Path, const std::string& FileName);
        Index(const std::filesystem::directory_entry& dirent);
        Index(const Index&);
        Index& operator = (const Index&);
        virtual ~Index();

        void Set(const Path& path, const std::string& fileName);
        bool Exists(void) const;
        bool Copy(const Index& Destination, std::filesystem::copy_options copyOptions = std::filesystem::copy_options::none) const;
        bool Delete() const;
        bool Move(const Index& Destination) const;
        bool Rename( const Index& Destination );
        bool Resize( std::uintmax_t newSize );
        bool IsBlockFile() const;
        bool IsCharacterFile() const;
        bool IsDirectory() const;
        bool IsFIFO() const;
        bool IsOther() const;
        bool IsRegularFile() const;
        bool IsSocket() const;
        bool IsSymLink() const;
        std::uintmax_t Size() const;
        std::filesystem::file_time_type LastWriteTime() const;
        std::filesystem::file_status Status() const;
        std::filesystem::file_status SymLinkStatus() const;
        std::string ToString() const;
        Path GetPath(void) const;
        
    private:
        std::filesystem::directory_entry m_dirent;
    };

}}}//qor::io::filesystem

#endif//QOR_PP_H_PLATFORM_FILESYSTEM_FILEINDEX
