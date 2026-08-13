// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/framework/data/pipeline/podbuffer.h"
#include "src/framework/data/pipeline/pipeline.h"
#include "src/components/data/formats/text/parser/nodes/textnode.h"
#include "src/components/data/formats/text/parser/textparser.h"
#include "src/components/data/pipeline/sinks/parsersink/parsersink.h"
#include "src/components/data/parser/parser.h"
#include "src/components/io/pipeline/connectors/fileconnector/fileconnector.h"
#include "src/components/data/pipeline/filters/unicodetextfilter/textfilter.h"
/*
*/
class TextReader
{
public:

    TextReader() : m_sink(m_textFilter){ }

    qor::ref_of<std::string>::type operator()(const qor::io::filesystem::Index& inFile)
    {
        return operator()(qor::io::components::FileConnector(inFile, m_textFilter, qor::io::OpenFor::ReadOnly, qor::io::WithFlags::None));
    }

    qor::ref_of<std::string>::type operator()(const qor::pipeline::Plug& sourceConnector)
    {           
        m_textFilter.Reset(1, 1);
        m_sink.Reset();
        qor::pipeline::Pipeline (sourceConnector, m_sink, qor::pipeline::Element::Push).InsertInlineFilter(m_textFilter).Connect().PumpAll();
        m_sink.Parser().FinalParse();
        auto finalNode = m_sink.Parser().PopNode();
        return finalNode.AsRef<qor::data::parser::text::TextNode>()->GetObject();
    }

    const qor::pipeline::Buffer& Buffer()
    {
        return m_textFilter;
    }

private:
    
    qor::text::components::UnicodeTextFilter m_textFilter;
    qor::pipeline::components::ParserSink<qor::data::parser::text::text>m_sink;
};
