// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/platform/platform.h"
#include "src/framework/data/pipeline/podbuffer.h"
#include "src/framework/data/pipeline/pipeline.h"
#include "src/components/io/pipeline/sinks/stdoutsink/stdoutsink.h"
#include "src/components/io/pipeline/connectors/fileconnector/fileconnector.h"
#include "src/components/io/pipeline/connectors/fileconnector/filesource.h"
#include "src/framework/io/filesystem/filesystem.h"
#include "linebylineworkflow.h"
#include "src/components/data/pipeline/filters/linereader/linereader.h"

using namespace qor;
using namespace qor::io;
using namespace qor::workflow;
using namespace qor::pipeline;
using namespace qor::platform;
using namespace qor::io::components;
using namespace qor::pipeline::components;

bool requiresFileSystem = ImplementsIFileSystem();

LineByLineWorkflow::LineByLineWorkflow() : state0(new_ref<qor::workflow::State>(this))
{
    state0->Enter = [this]()->void{
                
        Pipeline testPipeline;
        testPipeline.SetFlowMode(Element::FlowMode::Push);
        ByteBuffer InBuffer(1024);
        ByteBuffer OutBuffer(1024);

        StdOutSink sink;
        sink.SetParent(&testPipeline);
        sink.SetBuffer(&OutBuffer);    

        FileSource source;
        source.SetBuffer(&InBuffer);
        source.SetParent(&testPipeline);    

        FileConnector connector;
        source.SetPlug(&connector);
    
        auto filesystem = ThePlatform(qor_shared)->GetSubsystem<FileSystem>();
        io::filesystem::Index newIndex(filesystem->CurrentPath(), "linebyline_source.txt");

        LineReader filter;
        filter.SetFlowMode(Element::FlowMode::Push);
        filter.SetParent(&testPipeline);
        filter.SetSink(&sink);
        filter.SetSource(&source);

        testPipeline.SetSource(&source);
        testPipeline.SetSink(&sink);

        connector.SetFile(newIndex);
        connector.SetOpenFor(OpenFor::ReadOnly);
        connector.Connect();

        size_t unitsPumped = 0;
        do
        {
            unitsPumped = 0;
            testPipeline.PumpSome(unitsPumped, 1024);
        } while (unitsPumped > 0);
        
        SetComplete();
    };

    SetInitialState(state0);
}