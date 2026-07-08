// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/platform/compiler/compiler.h"
#include "src/components/protocols/http/server/requestparser/requesttoken.h"
#include "src/components/protocols/http/server/requestparser/nodes/requestnode.h"
#include "src/components/data/pipeline/sinks/parsersink/parsersink.h"
#include "src/framework/data/pipeline/podbuffer.h"
#include "src/framework/data/pipeline/pipeline.h"

class RequestReader
{
public:

    RequestReader() : m_byteBuffer(2048), m_sink(m_byteBuffer){ }

    qor::ref_of<qor::components::protocols::http::HTTPRequest>::type operator()(const qor::pipeline::Plug& sourceConnector)
    {        
        qor::pipeline::Pipeline(
            sourceConnector,
            m_sink,
            qor::pipeline::Element::Push).Connect().PumpAll();
      
        m_sink.Parser().FinalParse();
        auto finalNode = m_sink.Parser().PopNode();
        return finalNode.AsRef<qor::components::protocols::http::RequestNode>()->GetObject();
    }

    const qor::pipeline::Buffer& Buffer()
    {
        return m_byteBuffer;
    }

private:

    qor::pipeline::PODBuffer<qor::byte> m_byteBuffer;
    qor::pipeline::components::ParserSink<qor::components::protocols::http::request> m_sink;
};
