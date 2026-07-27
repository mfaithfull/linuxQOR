// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_LINUX_SYSTEM_FILESYSTEM_PIPE
#define QOR_PP_H_OS_LINUX_SYSTEM_FILESYSTEM_PIPE

#include "src/framework/io/filesystem/pipe/pipe.h"
#include "src/framework/io/filesystem/fileindex.h"
#include "src/framework/io/filesystem/path.h"
#include "src/platform/os/linux/io/async/asyncioservice.h"

//Declaration must match the one in src/system/filesystem/ifile.h
namespace qor{ bool qor_pp_module_interface(QOR_LINUXFILESYSTEM) ImplementsPipe(); }

//All types on this interface must be portable
namespace qor{ namespace io { namespace lin{

    class qor_pp_module_interface(QOR_LINUXFILESYSTEM) Pipe : public io::Pipe
    {
    public:

        Pipe();
        Pipe(const Pipe& src);
        Pipe(const filesystem::Path& path, const std::string& fileName, int openFor, int withFlags) : io::Pipe(filesystem::Index(path,fileName),openFor,withFlags) {}
        Pipe(const filesystem::Index& direntry, int openFor, int withFlags);
        Pipe(int fd);
        virtual ~Pipe();

        virtual task<int> AsyncRead(const qor::io::async::Interface& ioContext, byte* buffer, size_t byteCount, off_t offset) override;
        virtual task<int> AsyncWrite(const qor::io::async::Interface& ioContext, byte* buffer, size_t byteCount, off_t offset) override;
        virtual int64_t Read(byte* buffer, size_t byteCount, int64_t offset = -1) override;
        virtual int64_t Write(byte* buffer, size_t byteCount, int64_t offset = -1) override;

    private:

        int SyncToSystem();
        int GetDescriptor() const;
        int ChangeAccess(unsigned int mode);
        int GetOperatingMode();
        int ChangeDescriptorMode(int flags);
        int GetDescriptorMode();
        int AdviseOnUsage(off_t offset, off_t length, int advise);
        int ReserveSpace(off_t offset, off_t length);
        int ChangeOperatingMode(int flags);

        static int64_t Validate_write_Result(int64_t result);
        static int64_t Validate_read_Result(int64_t result);
        static off_t Validate_lseek_Result(off_t result);
        static uint64_t Validate_lseek64_Result(uint64_t result);
        static int Validate_ftruncate_Result(int result);
        static int Validate_posix_fallocate_Result(int result);
        static int Validate_fcntl_Result(int result);
        static int Validate_posix_fadvise_Result(int result);
        static int Validate_fchmod_Result(int result);
        static int Validate_fsync_Result(int result);
        static void Check_fsync_Result(int result);
        static void Check_close_Result(int result);
        static void ErrorOnOpen(int err);

    };
}}}//qor::io::lin

#endif//QOR_PP_H_OS_LINUX_SYSTEM_FILESYSTEM_PIPE
