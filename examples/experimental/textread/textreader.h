// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/framework/data/pipeline/podbuffer.h"
#include "src/framework/data/pipeline/pipeline.h"
#include "src/components/data/formats/text/parser/nodes/textnode.h"
#include "src/components/data/formats/text/parser/textparser.h"
#include "src/components/data/pipeline/sinks/parsersink/parsersink.h"
#include "src/components/data/parser/parser.h"
#include "src/components/io/pipeline/connectors/fileconnector/fileconnector.h"
/*
template< class TextPartObjectT, class TextModelObjectT >
class TextPartReader
{
public:

    TextPartReader() : m_byteBuffer(12), m_sink(m_byteBuffer){ }

    qor::ref_of<TextModelObjectT>::type operator()(const qor::io::filesystem::Index& inFile)
    {
        return operator()(qor::io::components::FileConnector(inFile, m_byteBuffer, qor::io::OpenFor::ReadOnly, qor::io::WithFlags::None));
    }

    qor::ref_of<TextModelObjectT>::type operator()(const qor::pipeline::Plug& sourceConnector)
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
        return finalNode.template AsRef< qor::data::parser::NodeAdapter< TextModelObjectT > >()->GetObject();
    }

    const qor::pipeline::Buffer& Buffer()
    {
        return m_byteBuffer;
    }

private:

    qor::pipeline::PODBuffer<qor::byte> m_byteBuffer;
    qor::pipeline::components::ParserSink< TextPartObjectT > m_sink;

};
*/
class TextReader
{
public:

    TextReader() : m_byteBuffer(1), m_sink(m_byteBuffer){ }

    qor::ref_of<std::string>::type operator()(const qor::io::filesystem::Index& inFile)
    {
        return operator()(qor::io::components::FileConnector(inFile, m_byteBuffer, qor::io::OpenFor::ReadOnly, qor::io::WithFlags::None));
    }

    qor::ref_of<std::string>::type operator()(const qor::pipeline::Plug& sourceConnector)
    {           
        m_byteBuffer.Reset();
        m_sink.Reset();
        qor::pipeline::Pipeline(sourceConnector, m_sink, qor::pipeline::Element::Push).Connect().PumpAll();
        m_sink.Parser().FinalParse();
        auto finalNode = m_sink.Parser().PopNode();
        return finalNode.AsRef<qor::data::parser::text::TextNode>()->GetObject();
    }

    const qor::pipeline::Buffer& Buffer()
    {
        return m_byteBuffer;
    }

private:

    qor::pipeline::PODBuffer<qor::byte> m_byteBuffer;
    qor::pipeline::components::ParserSink<qor::data::parser::text::text>m_sink;
};
