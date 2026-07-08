// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/framework/data/pipeline/podbuffer.h"
#include "src/framework/data/pipeline/pipeline.h"
#include "src/components/protocols/json/parser/nodes/object.h"
#include "src/components/protocols/json/parser/_3/object.h"
#include "src/components/data/pipeline/sinks/parsersink/parsersink.h"
#include "src/components/data/parser/parser.h"
#include "src/components/io/pipeline/connectors/fileconnector/fileconnector.h"

template< class JSONPartObjectT, class JSONModelObjectT >
class JSONPartReader
{
public:

    JSONPartReader() : m_byteBuffer(12), m_sink(m_byteBuffer){ }

    qor::ref_of<JSONModelObjectT>::type operator()(const qor::io::filesystem::Index& inFile)
    {
        return operator()(qor::io::components::FileConnector(inFile, m_byteBuffer, qor::io::OpenFor::ReadOnly, qor::io::WithFlags::None));
    }

    qor::ref_of<JSONModelObjectT>::type operator()(const qor::pipeline::Plug& sourceConnector)
    {           
        m_byteBuffer.Reset();
        m_sink.Reset();
        m_byteBuffer.Reset();
        size_t unitsPumped = qor::pipeline::Pipeline(sourceConnector, m_sink, qor::pipeline::Element::Push).Connect().PumpAll();
        if(unitsPumped > 0)
        {
            m_sink.Parser().FinalParse();
        }
        else
        {
            qor::log::Debug("No more data");
        }
        auto finalNode = m_sink.Parser().PopNode();
        return finalNode.template AsRef< qor::components::parser::NodeAdapter< JSONModelObjectT > >()->GetObject();
    }

    const qor::pipeline::Buffer& Buffer()
    {
        return m_byteBuffer;
    }

private:

    qor::pipeline::PODBuffer<qor::byte> m_byteBuffer;
    qor::pipeline::components::ParserSink< JSONPartObjectT > m_sink;

};

class JSONReader
{
public:

    JSONReader() : m_byteBuffer(2048), m_sink(m_byteBuffer){ }

    qor::ref_of<qor::components::model::json::Object>::type operator()(const qor::io::filesystem::Index& inFile)
    {
        return operator()(qor::io::components::FileConnector(inFile, m_byteBuffer, qor::io::OpenFor::ReadOnly, qor::io::WithFlags::None));
    }

    qor::ref_of<qor::components::model::json::Object>::type operator()(const qor::pipeline::Plug& sourceConnector)
    {           
        m_byteBuffer.Reset();
        m_sink.Reset();
        qor::pipeline::Pipeline(sourceConnector, m_sink, qor::pipeline::Element::Push).Connect().PumpAll();
        m_sink.Parser().FinalParse();
        auto finalNode = m_sink.Parser().PopNode();
        return finalNode.AsRef<qor::components::parser::json::ObjectNode>()->GetObject();
    }

    const qor::pipeline::Buffer& Buffer()
    {
        return m_byteBuffer;
    }

private:

    qor::pipeline::PODBuffer<qor::byte> m_byteBuffer;
    qor::pipeline::components::ParserSink<qor::components::parser::json::object> m_sink;
};
