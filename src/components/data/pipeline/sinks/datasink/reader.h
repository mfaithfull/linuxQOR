// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SINKS_READER
#define QOR_PP_H_COMPONENTS_PIPELINE_SINKS_READER

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
#include "datasink.h"
#include "src/qor/reflection/reflection.h"

namespace qor{ namespace pipeline {

    class Reader
    {

    public:

        inline Reader(const Plug& sourceConnection, size_t bufferSize) : m_buffer(bufferSize)
        {
            SetupPipeline(dynamic_cast<Source*>(sourceConnection.GetSource()));
        }

        inline Reader(Source& source, size_t bufferSize) : m_buffer(bufferSize)
        {
            SetupPipeline(&source);
        }

        inline virtual ~Reader() = default;

        inline ByteBuffer& GetBuffer()
        {
            return m_buffer;
        }
            
        inline void SetByteOrder(const arch::Endian byteOrder)
        {
            m_sink.SetByteOrder(byteOrder);
        }        

        arch::Endian GetByteOrder()
        {
            return m_sink.GetByteOrder();
        }

        template <typename T> requires std::is_arithmetic_v<T>
        inline T Read()
        {
            T t{0};
            m_sink.Extract(t);
            return t;
        }

        inline bool Read(byte* data, size_t byteCount)
        {
            return m_sink.Extract(data, byteCount);
        }

        template <typename T> requires std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T>
        inline Reader& operator >> (T& item)
        {
            m_sink.Extract(item);
            return *this;
        }

        template <typename T> requires std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T>
        inline T ReadStructure()
        {
			T result{0};
			qor_reflection::for_each_field(result, [this](auto& value, std::size_t /*i*/)
				{
                    *this >> (value);
				}
			);
            return result;
        }

    protected:

        DataSink m_sink;
        ByteBuffer m_buffer;
        Pipeline m_pipeline;

    private:

        void SetupPipeline(Source* source)
        {
            m_sink.SetBuffer(m_buffer);
            source->SetBuffer(m_buffer);
            m_sink.SetSource(source);
            source->SetSink(&m_sink);
            m_pipeline.SetSource(source);
            m_pipeline.SetSink(&m_sink);
            m_pipeline.SetFlowMode(Element::Pull);
            m_sink.SetCallback(Delegate<bool(size_t&,size_t)>::Create<Pipeline, &Pipeline::Pump>(m_pipeline));
            m_pipeline.Connect();
        }    
    };
}}

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SINKS_READER
