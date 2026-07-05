// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_FILESYSTEM_FILESYSTEM
#define QOR_PP_H_PLATFORM_FILESYSTEM_FILESYSTEM

#include <filesystem>
#include "ifilesystem.h"
#include "root.h"
#include "file/file.h"
#include "src/platform/isubsystem.h"

namespace qor
{
#ifdef QOR_FILESYSTEM    
    bool qor_pp_import ImplementsIFileSystem();
#else
    bool qor_pp_export ImplementsIFileSystem();//All libraries providing an implementation of IFileSystem need to export this function so that the linker can find them
#endif
}

namespace qor{ namespace io{

    class qor_pp_module_interface(QOR_FILESYSTEM) FileSystem : public ISubsystem
    {
    public:

        typedef ref_of<FileSystem>::type ref;

        FileSystem();
        virtual ~FileSystem() noexcept;

        virtual void Setup();
        virtual void Shutdown();

        const filesystem::Root& GetRoot() const;
        filesystem::Path CurrentPath() const;
        void CurrentPath(filesystem::Path& path) const;

        ref_of<File>::type Create(const filesystem::Index& index, const int withFlags) const;
        std::optional<filesystem::Folder> Create(const filesystem::Path& path) const;
        std::optional<filesystem::Folder> MakeDir(const filesystem::Path& path) const;
        std::optional<filesystem::Folder> NewFolder(const filesystem::Path& path) const;
        ref_of<File>::type Open(const filesystem::Index& index, const int openFor = OpenFor::ReadWrite, const int withFlags = 0) const;
        bool Delete(const filesystem::Index& index) const;
        bool RemoveDir(const filesystem::Path& path) const;
        bool DeleteFolder(const filesystem::Path& path) const;
        bool Copy(const filesystem::Index& srcIndex, const filesystem::Index& destIndex) const;
        bool Move(const filesystem::Index& srcIndex, const filesystem::Index& destIndex) const;
        bool Rename(filesystem::Index& srcIndex, const filesystem::Index& destIndex) const;

        std::filesystem::space_info Space(const filesystem::Path& path) const;
        filesystem::Path TempFolder() const;
        filesystem::Path ApplicationLogPath() const;
                
    private:

        ref_of<IFileSystem>::type m_pimpl;
        filesystem::Root m_root;
    };
    
    }//qor::io
    
    constexpr GUID FileSystemGUID = {0x3eea49d1, 0x3bc7, 0x429b, {0xb8, 0x0a, 0x70, 0xe3, 0xcf, 0x87, 0x05, 0xe0}};
    qor_pp_declare_guid_of(io::FileSystem,FileSystemGUID);
}//qor

#endif//QOR_PP_H_PLATFORM_FILESYSTEM_FILESYSTEM