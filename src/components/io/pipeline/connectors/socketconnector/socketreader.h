// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SOURCE_SOCKETREADER
#define QOR_PP_H_COMPONENTS_PIPELINE_SOURCE_SOCKETREADER

#include <functional>
#include "src/framework/data/pipeline/pipeline.h"
#include "src/framework/data/pipeline/podbuffer.h"
#include "socketconnector.h"
#include "socketsource.h"

namespace qor{ namespace io{ namespace network{ namespace components{

    class qor_pp_module_interface(QOR_SOCKETCONNECTOR) SocketReader : public pipeline::Pipeline
    {
    public:

        SocketReader();
        SocketReader(pipeline::Buffer* buffer);
        virtual ~SocketReader();

        virtual void SetBuffer(pipeline::Buffer* buffer);
        void SetBufferCapacity(size_t itemCount);
        void SetPlug(ref_of<SocketConnector>::type plug);
        virtual bool Read(size_t& unitsRead, size_t unitsToRead);
        ref_of<qor::io::network::Socket>::type Socket();

    protected:

        virtual bool Write(size_t& /*unitsWritten*/, size_t /*unitstoWrite*/);
        ref_of<SocketConnector>::type m_connector;
        SocketSource m_source;
        pipeline::NullSink m_sink;
    };

}}}}//qor::io::network::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SOURCE_SOCKETREADER
