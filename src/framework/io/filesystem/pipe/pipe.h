// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_FILESYSTEM_PIPE
#define QOR_PP_H_PLATFORM_FILESYSTEM_PIPE

#include <string>
#include "../path.h"
#include "ipipe.h"
#include "src/framework/io/iodescriptor.h"
#include "src/framework/io/filesystem/fileindex.h"
#include "src/framework/io/network/sockets.h"

namespace qor{ namespace io{

    class qor_pp_module_interface(QOR_FILESYSTEM) Pipe : public Descriptor
	{
	public:

        Pipe();
        Pipe(int fd);
        Pipe(const Descriptor& descriptor);
        Pipe(const Pipe& src);
        Pipe(const filesystem::Index& index);
        Pipe(const filesystem::Index& index, const network::sockets::eType& Type);
        Pipe& operator = (const Pipe&);
        virtual ~Pipe();

        virtual task<int> AsyncRead(const qor::io::async::Interface& ioContext, byte* buffer, size_t byteCount, off_t offset);
        virtual task<int> AsyncWrite(const qor::io::async::Interface& ioContext, byte* buffer, size_t byteCount, off_t offset);
        virtual int64_t Read(byte* buffer, size_t byteCount, int64_t offset = -1);
        virtual int64_t Write(byte* buffer, size_t byteCount, int64_t offset = -1);

        static ref_of<Pipe>::type Open(const filesystem::Index& index);

    protected:

        filesystem::Index m_index;
    };
    }//io

    qor_pp_declare_factory_of(io::Pipe, ExternalFactory);                              
    constexpr GUID PipeGUID = {0x46DD4605, 0x28CD, 0x4BC7, {0x96, 0x6A, 0xD5, 0x2C, 0x11, 0x76, 0xC2, 0x18}};
    qor_pp_declare_guid_of(io::Pipe,PipeGUID);

}//qor

#endif//QOR_PP_H_PLATFORM_FILESYSTEM_PIPE
