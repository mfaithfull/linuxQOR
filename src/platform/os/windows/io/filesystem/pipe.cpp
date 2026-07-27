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
        m_handle = INVALID_HANDLE_VALUE;
    }

    Pipe::Pipe(const io::Descriptor& iod)
    {
        m_handle = iod.m_handle;
    }

    Pipe::Pipe(const Pipe& src) : File()
    {
        m_handle = src.m_handle;
        m_objectType = src.m_objectType;
    }

    Pipe::Pipe(const filesystem::Index& direntry, int openFor, int withFlags) : io::File(direntry)
    {
        unsigned long desiredAccess = GetDesiredAccess(openFor, withFlags);
        unsigned long shareMode = GetShareMode(openFor, withFlags);
        unsigned long creationDisposition = GetCreationDisposition(openFor, withFlags);
        unsigned long flagsAndAttributes = GetFlagsAndAttributes(openFor, withFlags);

        m_handle = this->Create(direntry.ToString().c_str(), desiredAccess, shareMode, nullptr, creationDisposition, flagsAndAttributes, nullptr);
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
        if(offset != -1)
        {
            SetPosition(offset);
        }
        unsigned long numberOfBytesRead = 0;
        if(!Kernel32::ReadFile(m_handle, buffer, (unsigned long)byteCount, &numberOfBytesRead, nullptr))
        {
            continuable("Read failed.");
        }
        return numberOfBytesRead;
    }

    int64_t Pipe::Write(byte* buffer, size_t byteCount, int64_t offset)
    {
        if(offset != -1)
        {
            SetPosition(offset);
        }
        unsigned long numberofBytesWritten = 0;
        if(!Kernel32::WriteFile(m_handle, buffer, (unsigned long)byteCount, &numberofBytesWritten, nullptr))
        {
            continuable("Write failed.");
        }
        return numberofBytesWritten;
    }

}}}//qor::io::win