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

		enum class Access : unsigned long
		{
			Inbound = 				0x00000001,
			Outbound = 				0x00000002,
			WriteDAC = 				0x00040000,	//The caller will have write access to the named pipe's discretionary access control list (ACL).
			WriteOwner =			0x00080000, //The caller will have write access to the named pipe's owner.
			AccessSystemSecurity = 	0x01000000, //The caller will have write access to the named pipe's SACL
		};

		enum class Flags : unsigned long
		{
			Firstinstance = 0x00080000,
			WriteThrough  = 0x80000000,
			Overlapped =  	0x40000000,
		};

		enum class Type : unsigned long
		{
			Byte	= 0x00000000, //Data is written to the pipe as a stream of bytes. This mode cannot be used with PIPE_READMODE_MESSAGE. The pipe does not distinguish bytes written during different write operations.
			Message = 0x00000004, //Data is written to the pipe as a stream of messages. The pipe treats the bytes written during each write operation as a message unit. The GetLastError function returns ERROR_MORE_DATA when a message is not read completely. This mode can be used with either PIPE_READMODE_MESSAGE or PIPE_READMODE_BYTE.
		};

		enum class ReadMode : unsigned long
		{
			Byte	= 0x00000000, //Data is read from the pipe as a stream of bytes. This mode can be used with either PIPE_TYPE_MESSAGE or PIPE_TYPE_BYTE.
			Message = 0x00000002,
		};

		enum class WaitMode : unsigned long
		{
			Wait	= 0x00000000, //Blocking mode is enabled. When the pipe handle is specified in the ReadFile, WriteFile, or ConnectNamedPipe function, the operations are not completed until there is data to read, all data is written, or a client is connected. Use of this mode can mean waiting indefinitely in some situations for a client process to perform an action.
			NoWait 	= 0x00000001, //Nonblocking mode is enabled. In this mode, ReadFile, WriteFile, and ConnectNamedPipe always return immediately.
			//Note that nonblocking mode is supported for compatibility with Microsoft LAN Manager version 2.0 and should not be used to achieve asynchronous I/O with named pipes. For more information on asynchronous pipe I/O, see Synchronous and Overlapped Input and Output.
		};

		enum class ClientMode : unsigned long
		{
			AcceptRemoteClients = 0x00000000, //Connections from remote clients can be accepted and checked against the security descriptor for the pipe.
			RejectRemoteClients = 0x00000008,
		};

        Pipe();
        Pipe(const Pipe& src);
        Pipe(const string_t& name, int openFor, int withFlags);
        Pipe(const Descriptor& iod);
        virtual ~Pipe();

		bool Disconnect();
		bool GetClientComputerName(string_t& clientComputerName);
		bool GetClientProcessId(unsigned long& clientProcessId);
		bool GetClientSessionId(unsigned long& clientSessionId);
		bool GetHandleState(unsigned long& state, unsigned long& curInstances, unsigned long& maxCollectionCount, unsigned long& collectDataTimeout, string_t& userName);
		bool GetInfo(unsigned long& flags, unsigned long& outBufferSize, unsigned long& inBufferSize, unsigned long& maxInstances);
		bool GetServerProcessId(unsigned long& serverProcessId);
		bool GetServerSessionId(unsigned long& serverSessionId);
		bool ImpersonateClient();
		bool Peek(byte* buffer, unsigned long bufferByteCount, unsigned long& bytesRead, unsigned long& totalBytesAvail, unsigned long& bytesLeftThisMessage);
		bool SetHandleState(unsigned long& mode, unsigned long& maxCollectionCount, unsigned long& collectDataTimeout);
		bool Transact(byte* inBuffer, unsigned long inBufferByteCount, byte* outBuffer, unsigned long outBufferByteCount, unsigned long& bytesRead, void* overlapped);
		bool Wait(unsigned long timeOut);

        virtual task<int> AsyncRead(const qor::io::async::Interface& ioContext, byte* buffer, size_t byteCount, off_t offset) override;
        virtual task<int> AsyncWrite(const qor::io::async::Interface& ioContext, byte* buffer, size_t byteCount, off_t offset) override;
        virtual int64_t Read(byte* buffer, size_t byteCount, int64_t offset = -1) override;
        virtual int64_t Write(byte* buffer, size_t byteCount, int64_t offset = -1) override;

    private:

		unsigned long GetAccess(int openFor);
		unsigned long GetPipeMode(int withFlags);

		string_t m_name;
		void* m_readHandle{nullptr};
		void* m_writeHandle{nullptr};

    };
}}}//qor::io::win

#endif//QOR_PP_H_WINDOWS_FILESYSTEM_PIPE
