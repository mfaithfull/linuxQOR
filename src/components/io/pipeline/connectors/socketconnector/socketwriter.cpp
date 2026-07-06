// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "socketwriter.h"

namespace qor{ namespace io{ namespace network{ namespace components{

    SocketWriter::SocketWriter() : pipeline::Pipeline()
    {
        m_sink.SetParent(this);
        m_sink.SetSource(this);
        this->SetSink(&m_sink);
        this->SetSource(&m_source);
    }

    SocketWriter::SocketWriter( const std::string &host, int port, const std::string &ip, qor::io::network::sockets::eAddressFamily address_family, qor::io::network::addrinfo_flags socket_flags, bool tcp_nodelay, bool ipv6_v6only, time_t timeout_sec, size_t bufferByteCount) : SocketWriter()
    {
        SetBufferCapacity(bufferByteCount);
        if(m_connector.IsNull())
        {
            SetPlug(new_ref<SocketConnector>());
        }
        m_connector->ConnectToAddress(host, port, ip, address_family, socket_flags, tcp_nodelay, ipv6_v6only, timeout_sec);
    }

    void SocketWriter::SetPlug(ref_of<SocketConnector>::type plug)
    {
        m_connector = plug;
        m_sink.SetPlug(plug);
    }

    void SocketWriter::SetBuffer(pipeline::Buffer* buffer)
    {
        Pipeline::SetBuffer(buffer);
        m_source.SetBuffer(buffer);
        m_sink.SetBuffer(buffer);
    }

    void SocketWriter::SetBufferCapacity(size_t itemCount)
    {
        auto buffer = GetBuffer();
        if(buffer)
        {
            buffer->SetCapacity(itemCount);
        }
    }

    bool SocketWriter::Write(size_t& unitsWritten, size_t unitsToWrite)
    {
        return ActualSink() ? ActualSink()->Write(unitsWritten, unitsToWrite) : false;
    }

    template<>
    uint16_t SocketWriter::HToN(const uint16_t& s)
    {
        return s;//TODO ::htons(s);
    }

    template<>
    uint32_t SocketWriter::HToN(const uint32_t& s)
    {
        return s;//TODO::htonl(s);
    }

    template<>
    uint64_t SocketWriter::HToN(const uint64_t& s)
    {

        return s;//TODO((uint64_t)htonl(s & 0xFFFFFFFF) << 32LL) | htonl(s >> 32);
    }

}}}}//qor::io::network::components
