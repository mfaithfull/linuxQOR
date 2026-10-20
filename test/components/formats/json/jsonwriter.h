// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEST_JSON_JSONWRITER
#define QOR_PP_H_TEST_JSON_JSONWRITER

//The write half of the JSON pipeline. This mirrors the read half in jsonreader.h:
//there a FileConnector at the source end feeds a parser state at the sink end,
//here a generator state at the source end feeds a FileConnector at the sink end.
//
//The source end is a GeneratorSource. It owns a Generator, the output Context
//that the generator writes into, and the initial generator state built from the
//model. On each pipeline pull it reserves space in the shared pipeline buffer
//and generates into it, so a model whose text is larger than the buffer is
//still written out completely.
//
//The sink end is a FileConnector, a pipeline::Plug that opens a file and writes
//whatever the buffer holds. Both ends are given the same pipeline::PODBuffer,
//which is what makes the generated text land in the file.

#include "src/framework/data/pipeline/pipeline.h"
#include "src/framework/data/pipeline/podbuffer.h"
#include "src/components/io/pipeline/connectors/fileconnector/fileconnector.h"
#include "src/components/data/pipeline/sources/generatorsource/generatorsource.h"
#include "src/components/data/formats/json/generator/states/_value.h"
#include "src/components/data/formats/json/generator/states/_object.h"
#include "src/components/data/formats/json/generator/states/_array.h"
#include "src/components/data/formats/json/model/value.h"
#include "src/components/data/formats/json/model/object.h"
#include "src/components/data/formats/json/model/array.h"

namespace qor {

    //Writes the JSON text form of one model element to a file and returns the
    //number of bytes the pipeline moved. bufferSize is the capacity of the
    //pipeline buffer the source generates into and the sink writes out of.
    //The model pointer is only used to build the initial generator state; the
    //source owns that state and drives it to completion.
    template< class JSONPartStateT, class JSONModelTypeT >
    class JSONPartWriter
    {
    public:

        JSONPartWriter(size_t bufferSize) : m_buffer(bufferSize){ }

        size_t operator()(const qor::io::filesystem::Index& outFile, JSONModelTypeT* model)
        {
            //WriteOnly with no mode flags leaves the platform to open the file
            //with CREATE_ALWAYS, so a missing file is created and an existing
            //one is truncated before anything is written to it.
            qor::io::components::FileConnector sinkConnector(outFile, m_buffer, qor::io::OpenFor::WriteOnly, qor::io::WithFlags::None);
            qor::pipeline::components::GeneratorSource<JSONPartStateT> source(m_buffer, model);
            return qor::pipeline::Pipeline(source, sinkConnector, qor::pipeline::Element::FlowMode::Push).Connect().PumpAll();
        }

        //The buffer both ends of the pipeline share.
        const qor::pipeline::Buffer& Buffer()
        {
            return m_buffer;
        }

    private:

        qor::pipeline::PODBuffer<byte> m_buffer;
    };

    //The writer for an object model, e.g. {"a":1}
    typedef JSONPartWriter<qor::data::generator::json::_object, qor::data::model::json::Object> JSONObjectWriter;

    //The writer for an array model, e.g. [1,2]
    typedef JSONPartWriter<qor::data::generator::json::_array, qor::data::model::json::Array> JSONArrayWriter;

    //The writer for a standalone value model, which routes to whichever state
    //writes the value it holds.
    typedef JSONPartWriter<qor::data::generator::json::_value, qor::data::model::json::Value> JSONValueWriter;

    //The top level writer, mirroring the JSONReader in jsonreader.h. It writes
    //the JSON text for an object model to a file.
    class JSONWriter
    {
    public:

        size_t operator()(const qor::io::filesystem::Index& outFile, qor::data::model::json::Object* object)
        {
            JSONObjectWriter writer(1024);
            return writer(outFile, object);
        }
    };

}//qor

#endif//QOR_PP_H_TEST_JSON_JSONWRITER
