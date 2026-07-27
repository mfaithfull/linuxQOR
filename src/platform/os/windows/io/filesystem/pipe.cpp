// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "pipe.h"
#include "src/framework/io/filesystem/ifilesystem.h"

#include "src/platform/os/windows/api_layer/kernel/kernel32.h"
#undef CreateFile

using namespace qor::win::api;
using namespace qor::platform::win;

//Export this trivial function so the linker will pull in this library to fulfil the ImplementsIFile requirement.
namespace qor{ bool qor_pp_module_interface(QOR_WINDOWSFILESYSTEM) ImplementsPipe() { return true; } }//qor

namespace qor{ namespace io { namespace win{

    Pipe::Pipe()
    {
        //Kernel32::CreatePipe(&m_readHandle, &m_writeHandle, nullptr, 0);
    }
    
    Pipe::Pipe(const io::Descriptor& iod)
    {
        m_handle = iod.m_handle;
    }

    Pipe::Pipe(const Pipe& src) : Pipe()
    {
        m_handle = src.m_handle;
    }

    unsigned long Pipe::GetAccess(int openFor)
    {
        return static_cast<unsigned long>(Pipe::Access::Inbound) | static_cast<unsigned long>(Pipe::Access::Outbound);
    }

    unsigned long Pipe::GetPipeMode(int withFlags)
    {
        return static_cast<unsigned long>(Pipe::ClientMode::AcceptRemoteClients) | static_cast<unsigned long>(Pipe::Flags::WriteThrough);
    }

    Pipe::Pipe(const string_t& name, int openFor, int withFlags) : io::Pipe()
    {
        string_t pipeName = _TXT("\\\\.\\pipe\\") + name;
        m_handle = Kernel32::CreateNamedPipe(name.data(), GetAccess(openFor), GetPipeMode(withFlags), PIPE_UNLIMITED_INSTANCES , 4, 4, 50, nullptr);
    }

    Pipe::~Pipe()
    {
        if(m_handle != nullptr && m_handle != INVALID_HANDLE_VALUE)
        {
            Kernel32::CloseHandle(m_handle);
        }
        m_handle = nullptr;
    }

    task<int> Pipe::AsyncRead(const qor::io::async::Interface& ioContext, byte* buffer, size_t byteCount, off_t offset)
    {
        return ioContext.Read(this, buffer, byteCount, offset);
    }

    task<int> Pipe::AsyncWrite(const qor::io::async::Interface& ioContext, byte* buffer, size_t byteCount, off_t offset)
    {
        return ioContext.Write(this, buffer, byteCount, offset);
    }

    int64_t Pipe::Read(byte* buffer, size_t byteCount, int64_t offset)
    {
        unsigned long numberOfBytesRead = 0;
        if(!Kernel32::ReadFile(m_handle, buffer, (unsigned long)byteCount, &numberOfBytesRead, nullptr))
        {
            continuable("Read failed.");
        }
        return numberOfBytesRead;
    }

    int64_t Pipe::Write(byte* buffer, size_t byteCount, int64_t offset)
    {
        unsigned long numberofBytesWritten = 0;
        if(!Kernel32::WriteFile(m_handle, buffer, (unsigned long)byteCount, &numberofBytesWritten, nullptr))
        {
            continuable("Write failed.");
        }
        return numberofBytesWritten;
    }

    bool Pipe::Disconnect()
    {
        return Kernel32::DisconnectNamedPipe(m_handle) ? true : false;
    }

    bool Pipe::GetClientComputerName(string_t& clientComputerName)
    {
        return Kernel32::GetNamedPipeClientComputerName(m_handle, clientComputerName.data(), static_cast<unsigned long>(clientComputerName.capacity())) ? true : false;
    }

    bool Pipe::GetClientProcessId(unsigned long& clientProcessId)
    {
        return Kernel32::GetNamedPipeClientProcessId(m_handle, &clientProcessId) ? true : false;
    }

    bool Pipe::GetClientSessionId(unsigned long& clientSessionId)
    {
        return Kernel32::GetNamedPipeClientSessionId(m_handle, &clientSessionId) ? true : false;
    }

    bool Pipe::GetHandleState(unsigned long& state, unsigned long& curInstances, unsigned long& maxCollectionCount, unsigned long& collectDataTimeout, string_t& userName)
    {
        return Kernel32::GetNamedPipeHandleState(m_handle, &state, &curInstances, &maxCollectionCount, &collectDataTimeout, userName.data(), static_cast<unsigned long>(userName.capacity()));
    }

    bool Pipe::GetInfo(unsigned long& flags, unsigned long& outBufferSize, unsigned long& inBufferSize, unsigned long& maxInstances)
    {
        return Kernel32::GetNamedPipeInfo(m_handle, &flags, &outBufferSize, &inBufferSize, &maxInstances) ? true : false;
    }

    bool Pipe::GetServerProcessId(unsigned long& serverProcessId)
    {
        return Kernel32::GetNamedPipeServerProcessId(m_handle, &serverProcessId) ? true : false;
    }

    bool Pipe::GetServerSessionId(unsigned long& serverSessionId)
    {
        return Kernel32::GetNamedPipeServerSessionId(m_handle, &serverSessionId) ? true : false;
    }

    bool Pipe::ImpersonateClient()
    {
        return Kernel32::ImpersonateNamedPipeClient(m_handle) ? true : false;
    }

    bool Pipe::Peek(byte* buffer, unsigned long bufferByteCount, unsigned long& bytesRead, unsigned long& totalBytesAvail, unsigned long& bytesLeftThisMessage)
    {
        return Kernel32::PeekNamedPipe(m_handle, buffer, bufferByteCount, &bytesRead, &totalBytesAvail, &bytesLeftThisMessage) ? true : false;
    }

    bool Pipe::SetHandleState(unsigned long& mode, unsigned long& maxCollectionCount, unsigned long& collectDataTimeout)
    {
        return Kernel32::SetNamedPipeHandleState(m_handle, &mode, &maxCollectionCount, &collectDataTimeout) ? true : false;
    }

    bool Pipe::Transact(byte* inBuffer, unsigned long inBufferByteCount, byte* outBuffer, unsigned long outBufferByteCount, unsigned long& bytesRead, void* overlapped)
    {
        return Kernel32::TransactNamedPipe(m_handle, inBuffer, inBufferByteCount, outBuffer, outBufferByteCount, &bytesRead, reinterpret_cast<LPOVERLAPPED>(overlapped)) ? true : false;
    }

    bool Pipe::Wait(unsigned long timeOut)
    {
        return Kernel32::WaitNamedPipe(m_name.data(), timeOut) ? true : false;
    }

}}}//qor::io::win