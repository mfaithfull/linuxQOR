// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_FILESYSTEM_FILE
#define QOR_PP_H_PLATFORM_FILESYSTEM_FILE

#include <string>
#include "../path.h"
#include "ifile.h"
#include "src/framework/io/iodescriptor.h"
#include "src/framework/io/filesystem/fileindex.h"

namespace qor{ namespace io{

    class qor_pp_module_interface(QOR_FILESYSTEM) File : public Descriptor
	{
	public:

        File();
        File(int fd);
        File(const Descriptor& descriptor);
        File(const File& src);
        File(const filesystem::Index& index);
        File(const filesystem::Index& index, int OpenFor, const int WithFlags = 0);
        File& operator = (const File&);
        virtual ~File();

        //implemented on File and Index. Equivalent, just uses file as a wrapper for Index
        virtual uint64_t GetSize();
        virtual uintmax_t GetHardLinkCount();
        virtual FileTime GetLastWriteTime();
        virtual filesystem::Permissions GetPermissions();
        virtual void SetPermissions(const filesystem::Permissions permissions, const filesystem::PermissionOptions options);
        virtual void ReSize(uintmax_t size);
        virtual FileStatus GetStatus();
        virtual FileStatus GetSymLinkStatus();
        virtual filesystem::Type GetType();

        virtual bool SupportsPosition();
        virtual uint64_t GetPosition();
        virtual long SetPosition(long offset, int whence);
        virtual uint64_t SetPosition(uint64_t newPosition);
        virtual uint64_t SetPositionRelative(int64_t offset);
        virtual void Truncate(uint64_t length);
        virtual void Reserve(uint64_t length);
        virtual void Flush();
        virtual ref_of<File>::type ReOpen(int openFor, int withFlags);
        virtual task<int> AsyncRead(const qor::io::async::Interface& ioContext, byte* buffer, size_t byteCount, off_t offset);
        virtual task<int> AsyncWrite(const qor::io::async::Interface& ioContext, byte* buffer, size_t byteCount, off_t offset);
        virtual int64_t Read(byte* buffer, size_t byteCount, int64_t offset = -1);
        virtual int64_t Write(byte* buffer, size_t byteCount, int64_t offset = -1);

        static ref_of<File>::type Open(const filesystem::Index& index, int openFor, int withFlags);

    protected:

        filesystem::Index m_index;
    };
    }//io

    qor_pp_declare_factory_of(io::File, ExternalFactory);
    constexpr GUID FileGUID = {0xFA2945F7, 0xEF60, 0x442F, {0x87, 0x1D, 0xD3, 0x9D, 0xED, 0xC1, 0xFE, 0x76}};
    qor_pp_declare_guid_of(io::File,FileGUID);

}//qor

#endif//QOR_PP_H_PLATFORM_FILESYSTEM_FILE
