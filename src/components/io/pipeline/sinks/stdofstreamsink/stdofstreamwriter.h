// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SINK_STDOFSTREAMWRITER
#define QOR_PP_H_COMPONENTS_PIPELINE_SINK_STDOFSTREAMWRITER

#include <functional>
#include "src/framework/data/pipeline/pipeline.h"
#include "src/framework/data/pipeline/podbuffer.h"
#include "src/framework/data/pipeline/sink.h"
#include "src/components/io/pipeline/connectors/stdfstreamconnector/stdifstreamconnector.h"
#include "stdofstreamsink.h"

namespace qor{ namespace io{ namespace components{ 

    class qor_pp_module_interface(QOR_STDOFSTREAMSINK) stdofstreamWriter : public pipeline::Pipeline
    {
    public:

        stdofstreamWriter();
        stdofstreamWriter(const std::string& fileName, size_t bufferByteCapacity = 4096);
        stdofstreamWriter(io::filesystem::Index& fileIndex, size_t bufferByteCapacity = 4096);
        virtual ~stdofstreamWriter() = default;

        void SetFile(io::filesystem::Index& fileIndex);
        void SetBufferCapacity(size_t itemCount);

        virtual bool Write(size_t& unitsWritten, size_t unitstoWrite);
        bool Write(const std::string& strData);

    protected:
        
        virtual bool Read(size_t& /*unitsRead*/, size_t /*unitsToRead*/ = 1) { return false; }
        void Setup();
        bool m_Setup;
        pipeline::ByteBuffer m_buffer;
        stdofstreamConnector m_connector;
        stdofstreamSink m_sink;
        pipeline::NullSource m_source;
    };

}}}//qor::io::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SINK_STDOFSTREAMWRITER
