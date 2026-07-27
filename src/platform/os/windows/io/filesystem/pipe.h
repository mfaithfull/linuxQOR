// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_FILESYSTEM_FILE
#define QOR_PP_H_WINDOWS_FILESYSTEM_FILE

#include "src/framework/io/filesystem/pipe/pipe.h"
#include "src/framework/io/filesystem/fileindex.h"
#include "src/framework/io/filesystem/path.h"
#include "src/platform/os/windows/common/structures.h"

//Declaration must match the one in src/system/filesystem/pipe/ipipe.h
namespace qor{ bool qor_pp_module_interface(QOR_WINDOWSFILESYSTEM) ImplementsPipe(); }

//All types on this interface must be portable
namespace qor{ namespace io { namespace win{    

    class qor_pp_module_interface(QOR_WINDOWSFILESYSTEM) Pipe : public io::Pipe
    {
    public:

        Pipe();
        Pipe(const Pipe& src);
        Pipe(const filesystem::Path& path, const std::string& fileName, int openFor, int withFlags) : Pipe(filesystem::Index(path,fileName),openFor,withFlags) {}
        Pipe(const filesystem::Index& direntry, int openFor, int withFlags);
        Pipe(const Descriptor& iod);
        virtual ~Pipe();

        virtual task<int> AsyncRead(const qor::io::async::Interface& ioContext, byte* buffer, size_t byteCount, off_t offset) override;
        virtual task<int> AsyncWrite(const qor::io::async::Interface& ioContext, byte* buffer, size_t byteCount, off_t offset) override;
        virtual int64_t Read(byte* buffer, size_t byteCount, int64_t offset = -1) override;
        virtual int64_t Write(byte* buffer, size_t byteCount, int64_t offset = -1) override;

    private:

		void* m_readHandle{nullptr};
		void* m_writeHandle{nullptr};

    };
}}}//qor::io::win

#endif//QOR_PP_H_WINDOWS_FILESYSTEM_PIPE
