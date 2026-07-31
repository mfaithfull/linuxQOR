// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "echoresponsefilter.h"
#include "src/qor/flyers/error/error.h"

using namespace qor;
using namespace qor::components;
using namespace qor::data;
using namespace qor::data::parser;
using namespace qor::fastflow;

namespace qor { namespace components { namespace protocols { namespace echo {

    //Client side protocol filter

    EchoResponseFilter::EchoResponseFilter() :  qor::pipeline::InlineFilter<qor::byte>(0) { }
    
    EchoResponseFilter::EchoResponseFilter(size_t itemCount) :  qor::pipeline::InlineFilter<qor::byte>(itemCount) { }

    EchoResponseFilter::~EchoResponseFilter() = default;

    void EchoResponseFilter::Filter(byte* space, byte* data, size_t& itemCount, size_t& writeCount)
    {
        EchoResponse response = Parse(data, itemCount);

        std::string output = response.GetValue();

        if(output == "quit")
        {
            continuable("user wants to quit");
        }

        size_t outputSize = output.size();
        if(outputSize > itemCount)
        {
            outputSize = itemCount;
        }
        itemCount = std::min(outputSize, writeCount);
        memcpy(space, output.data(), itemCount);
        writeCount = itemCount;
    }

    ref_of<EchoResponse>::type EchoResponseFilter::Parse(byte* data, size_t& itemCount)
    {
        Parser echoResponseParser(new_ref<Context>(data, itemCount));
        ref_of<response>::type responseState = new_ref<response>(&echoResponseParser);

        echoResponseParser.SetInitialStep(responseState.AsRef<Step>());
        echoResponseParser.Run();
        
        auto responseNode = echoResponseParser.PopNode().template AsRef<ResponseNode>();
        return responseNode->GetObject();
    }
}}}}//qor::components::protocols::echo
