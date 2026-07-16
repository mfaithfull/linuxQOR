// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCES_JSON_READER
#define QOR_PP_H_FRAMEWORK_RESOURCES_JSON_READER

#include "src/components/protocols/json/parser/tokens.h"
#include "src/components/protocols/json/parser/_3/object.h"
#include "src/components/protocols/json/parser/nodes/object.h"
#include "src/components/protocols/json/model/object.h"
#include "src/framework/data/pipeline/podbuffer.h"
#include "src/framework/data/pipeline/pipeline.h"
#include "src/components/data/pipeline/sinks/parsersink/parsersink.h"

namespace qor{ namespace framework{ namespace res {

    class JSONReader
    {
    public:

        JSONReader() : m_byteBuffer(2048), m_sink(m_byteBuffer){ }

        qor::ref_of<qor::components::model::json::Object>::type operator()(const qor::pipeline::Plug& sourceConnector)
        {
            qor::pipeline::Pipeline(
                sourceConnector,
                m_sink,
                qor::pipeline::Element::Push
            ).Connect().PumpAll();

            m_sink.Parser().FinalParse();
            auto finalNode = m_sink.Parser().PopNode();
            return finalNode;
        }

        const qor::pipeline::Buffer& Buffer()
        {
            return m_byteBuffer;
        }

    private:

        qor::pipeline::PODBuffer<byte> m_byteBuffer;
        qor::pipeline::components::ParserSink<qor::components::parser::json::object> m_sink;
    };

}}}//qor::framework::res

#endif//QOR_PP_H_FRAMEWORK_RESOURCES_JSON_READER