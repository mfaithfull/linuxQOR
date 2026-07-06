// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_WRITER
#define QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_WRITER

#include <string>
#include <type_traits>
#include "src/framework/data/pipeline/pipeline.h"
#include "src/framework/data/pipeline/podbuffer.h"
#include "src/framework/data/pipeline/source.h"
#include "src/framework/data/pipeline/sink.h"
#include "src/components/io/pipeline/connectors/fileconnector/fileconnector.h"
#include "src/platform/compiler/compiler.h"
#include "src/platform/architecture/cpu.h"
#include "src/framework/event/delegate/delegate.h"
#include "datasource.h"

namespace qor{ namespace pipeline{

    class Writer
    {
    public:

        inline Writer(const Plug& sinkConnection, size_t bufferSize) : m_buffer(bufferSize)
        {
            SetupPipeline(dynamic_cast<Sink*>(sinkConnection.GetSink()));
        }

        inline Writer(Sink& sink, size_t bufferSize) : m_buffer(bufferSize)
        {
            SetupPipeline(&sink);
        }

        virtual ~Writer() = default;

        inline ByteBuffer& GetBuffer()
        {
            return m_buffer;
        }
            
        inline void SetByteOrder(const arch::Endian byteOrder)
        {
            m_source.SetByteOrder(byteOrder);
        }        

        template <typename T> requires std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T>
        inline void Write(T& t)
        {
            m_source.Insert(t);        
        }

        template <typename T> requires std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T>
        inline Writer& operator << (T& item)
        {
            m_source.Insert(item);
            return *this;
        }

    protected:

        DataSource m_source;
        ByteBuffer m_buffer;
        ref_of<Pipeline>::type m_pipeline;

    private:

        void SetupPipeline(Sink* sink)
        {
            m_source.SetBuffer(m_buffer);
            sink->SetBuffer(m_buffer);
            m_source.SetSink(sink);
            sink->SetSource(&m_source);
            m_pipeline = new_ref<Pipeline>();
            m_pipeline->SetSink(sink);
            m_pipeline->SetSource(&m_source);
            m_pipeline->SetFlowMode(Element::Push);
            m_source.SetCallback(Delegate<bool(size_t&,size_t)>::Create<Pipeline, &Pipeline::Pump>(m_pipeline));
            m_pipeline->Connect();
        }
    };

}}//qor::pipeline

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_WRITER
