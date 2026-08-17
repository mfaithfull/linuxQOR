// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "pipe.h"

namespace qor{ namespace io{

    ref_of<Pipe>::type Pipe::Open(const filesystem::Index& index)
    {
        return new_ref<Pipe>(index);
    }

    //Base implementation of Pipe

    Pipe::Pipe(){ }

    Pipe::Pipe(int fd) : Pipe()
    {
        Descriptor::m_fd = fd;
    }

    Pipe::Pipe(const Descriptor& descriptor) : Descriptor(descriptor){ }

    Pipe::Pipe(const Pipe& src) : m_index(src.m_index)
    {
        *this = src;
    }

    Pipe::Pipe(const filesystem::Index& index) : m_index(index){ }

    Pipe::Pipe(const filesystem::Index& index, const network::sockets::eType& /*Type*/) : m_index(index){ }

    Pipe& Pipe::operator = (const Pipe& src)
    {
        if(&src != this)
        {
            m_index = src.m_index;
        }
        return *this;
    }

    Pipe::~Pipe(){/*derived class owns platform specific resource and handles close in its destructor*/}

    task<int> Pipe::AsyncRead(const qor::io::async::Interface& /*ioContext*/, byte* /*buffer*/, size_t /*byteCount*/, off_t /*offset*/)
    {
        serious("Empty base implementation.");
        return task<int>{};
    }

    task<int> Pipe::AsyncWrite(const qor::io::async::Interface& /*ioContext*/, byte* /*buffer*/, size_t /*byteCount*/, off_t /*offset*/)
    {
        serious("Empty base implementation.");
        return task<int>{};
    }

    int64_t Pipe::Read(byte* /*buffer*/, size_t /*byteCount*/, int64_t /*offset*/)
    {
        serious("Empty base implementation.");
        return 0;
    }

    int64_t Pipe::Write(byte* /*buffer*/, size_t /*byteCount*/, int64_t /*offset*/)
    {
        serious("Empty base implementation.");
        return 0;
    }

}}//qor::io
