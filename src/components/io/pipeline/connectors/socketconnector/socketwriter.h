// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SINK_SOCKETWRITER
#define QOR_PP_H_COMPONENTS_PIPELINE_SINK_SOCKETWRITER

#include <functional>
#include "src/framework/data/pipeline/pipeline.h"
#include "src/framework/data/pipeline/podbuffer.h"
#include "src/framework/data/pipeline/sink.h"
#include "socketconnector.h"
#include "socketsink.h"

namespace qor{ namespace io{ namespace network{ namespace components{

    class SocketWriter : public pipeline::Pipeline
    {
    public:

        SocketWriter();
        virtual ~SocketWriter() = default;

        SocketWriter(
            const std::string &host,
            int port,
            const std::string &ip,
            qor::io::network::sockets::eAddressFamily address_family = qor::io::network::sockets::eAddressFamily::AF_INet,
            qor::io::network::addrinfo_flags socket_flags = 0,
            bool tcp_nodelay = false, bool ipv6_v6only = false, time_t timeout_sec = 0, size_t bufferByteCount = 1024);

        virtual void SetBuffer(pipeline::Buffer* buffer);
        void SetBufferCapacity(size_t itemCount);
        void SetPlug(ref_of<SocketConnector>::type plug);
        virtual bool Write(size_t& unitsWritten, size_t unitstoWrite);
        ref_of<qor::io::network::Socket>::type Socket();

        template<typename T>
        bool Write(const T& hdata)
        {
            auto ndata = HToN(hdata);
            auto buffer = GetBuffer();
            if(buffer)
            {
                size_t unitSize = buffer->GetUnitSize();
                size_t totalUnits = (sizeof(T)/unitSize);
                size_t unitsWritten = 0;
                size_t unitsWrittenAtOnce;
                do
                {
                    unitsWrittenAtOnce = 0;
                    size_t unitsToWrite = totalUnits - unitsWritten;
                    byte* space = buffer->WriteRequest(unitsToWrite);
                    memcpy( space, GetAddressOfData(ndata) + (unitsWritten * unitSize), unitsToWrite);
                    buffer->WriteAcknowledge(unitsToWrite);
                    if(!Write(unitsWrittenAtOnce, unitsToWrite))
                    {
                        return false;
                    }
                    if(unitsWrittenAtOnce == 0)
                    {
                        break;
                    }
                    unitsWritten += unitsWrittenAtOnce;
                }while(unitsWritten < totalUnits);
                return true;
            }
            return false;
        }

    protected:

        template<typename T>
        T HToN(const T& t)
        {
            return t;
        }

        template<typename T>
        const void* GetAddressOfData(const T& t)
        {
            return (const void*)&const_cast<T&>(t);
        }

        virtual bool Read(size_t& /*unitsRead*/, size_t /*unitsToRead*/ = 1 ) { return false; }
        ref_of<SocketConnector>::type m_connector;
        SocketSink m_sink;
        pipeline::NullSource m_source;
    };

}}}}//qor::io::network::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SINK_SOCKETWRITER
