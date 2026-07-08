// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCES_TTF_READER
#define QOR_PP_H_FRAMEWORK_RESOURCES_TTF_READER

#include "src/platform/compiler/compiler.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/data/pipeline/podbuffer.h"
#include "src/framework/data/pipeline/pipeline.h"
#include "src/components/io/pipeline/sinks/deserializersink/deserializersink.h"
#include "src/components/io/deserializer/state.h"
#include "ttfobject.h"
#include "reader/designator.h"
#include "reader/ttfinitial.h"

namespace qor{ namespace framework{ namespace res { 

    class qor_pp_module_interface(QOR_RESOURCES) TTFReader
    {
    public:

        TTFReader() : m_byteBuffer(32767)
        {        
        }
        
        uint32_t ReadDesignator(const qor::pipeline::Plug& sourceConnector)
        {
            size_t unitsPumped = 0;
            qor::pipeline::components::DeserializerSink<DesignatorState> sink(m_byteBuffer);
            pipeline::Pipeline(
                sourceConnector,
                sink,
                qor::pipeline::Element::Push
            ).Connect().Pump(unitsPumped, sizeof(uint32_t));
            return sink.GetObject()->GetObject();            
        }

        ref_of<TTFObject>::type operator()(const qor::pipeline::Plug& sourceConnector)
        {                    
            pipeline::components::DeserializerSink<TTFInitialState> sink(m_byteBuffer);
            qor::pipeline::Pipeline(
                sourceConnector,
                sink,
                qor::pipeline::Element::Push
            ).Connect().PumpAll();
                    
            auto ttfState = sink.GetObject();
            
            return ref_of<TTFObject>::type();
        }

        const qor::pipeline::Buffer& Buffer()
        {
            return m_byteBuffer;
        }

    private:

        pipeline::PODBuffer<byte> m_byteBuffer;        
    };

}}}//qor::framework::res

#endif//QOR_PP_H_FRAMEWORK_RESOURCES_TTF_READER