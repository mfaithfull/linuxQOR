// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_FILESYSTEM_INTERFACE
#define QOR_PP_H_PLATFORM_FILESYSTEM_INTERFACE

#include <optional>
#include <filesystem>
#include "src/qor/memory/instance/singleton.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "src/qor/flyers/error/error.h"
#include "fileindex.h"
#include "folder.h"
#include "file/file.h"

namespace qor{ namespace io{

    enum OpenFor
    {
        Exec = 1,
        ReadOnly = 2,
        ReadWrite = 4,
        Search = 8,
        WriteOnly = 16,
    };

    enum WithFlags
    {
        None = 0u,
        Append = 1 << 0,
        CloseExec = 1 << 1,
        CloseFork = 1 << 2,
        CreateNew = 1 << 3,
        Directory = 1 << 4,
        DSync = 1 << 5,
        Exclusive = 1 << 6,
        NoCTTY = 1 << 7,
        NoFollow = 1 << 8,
        NonBlock = 1 << 9,
        RSync = 1 << 10,
        Sync = 1 << 11,
        Truncate = 1 << 12,
        TTYInit = 1 << 13,
        TempFile = 1 << 14,
    };

    class IFileSystem
    {
    public:

        inline IFileSystem() = default;
        inline virtual ~IFileSystem() noexcept = default;

        virtual void Setup() {}
        virtual void Shutdown() {}

        virtual std::string PathSeparator() const { return "/"; }
        virtual std::string SelfIndicator() const { return "."; }
        virtual std::string ParentIndicator() const { return ".."; }
        virtual std::string RootIndicator() const { return "/"; }
        virtual unsigned short MaxElementLength() const { return 256; }
        virtual ref_of<File>::type Create(const filesystem::Index& /*index*/, const int /*withFlags*/) const { ref_of<File>::type noresult; return noresult;}
        virtual ref_of<File>::type Open(const filesystem::Index& /*index*/, const int /*openFor*/, const int /*withFlags*/) const {ref_of<File>::type noresult; return noresult;}
        virtual bool Move(const filesystem::Index& /*srcIndex*/, const filesystem::Index& /*destIndex*/) const {return false;}
        virtual filesystem::Path CurrentPath() const {
            std::filesystem::path stdpath = std::filesystem::current_path();
            filesystem::Path currentPath(stdpath.string());
            return currentPath;
        }
        virtual filesystem::Path ApplicationLogPath() const 
        {
            return CurrentPath();
        }
    };
    
    }//qor::io
    
    qor_pp_declare_instancer_of(io::IFileSystem, SingletonInstancer);
    qor_pp_declare_factory_of(io::IFileSystem, ExternalFactory);
    constexpr GUID IFileSystemGUID = {0x3474967c, 0x0be1, 0x417d, { 0xab, 0x71, 0xd0, 0x21, 0x10, 0x16, 0x0e, 0x9f}};
    qor_pp_declare_guid_of(io::IFileSystem,IFileSystemGUID);

}//qor

#endif//QOR_PP_H_PLATFORM_FILESYSTEM_INTERFACE
