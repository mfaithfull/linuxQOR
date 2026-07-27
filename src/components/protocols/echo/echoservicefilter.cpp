// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "echoservicefilter.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/informative.h"

using namespace qor;
using namespace qor::components;
using namespace qor::data;
using namespace qor::data::parser;
using namespace qor::workflow;

namespace qor { namespace components { namespace protocols { namespace echo {

    //Server side protocol filter
    
    EchoServiceFilter::EchoServiceFilter() : qor::pipeline::InlineFilter<qor::byte>(0) { }
    
    EchoServiceFilter::EchoServiceFilter(size_t itemCount) : qor::pipeline::InlineFilter<qor::byte>(itemCount) { }

    EchoServiceFilter::~EchoServiceFilter() = default;

    void EchoServiceFilter::Filter(byte* space, byte* data, size_t& itemCount, size_t& writeCount)
    {
        EchoRequest request = Parse(data, itemCount);

        std::string input = request.GetValue();

        //qor::log::inform("Handling request, {0} bytes.", itemCount);
        if(input == "quit")
        {
            continuable("user wants to quit");
        }

        size_t inputSize = input.size();
        if(inputSize > itemCount)
        {
            inputSize = itemCount;
        }
        itemCount = std::min(inputSize,writeCount);
        memcpy(space, input.data(), itemCount);    
        writeCount = itemCount;    
    }

    ref_of<EchoRequest>::type EchoServiceFilter::Parse(byte* data, size_t& itemCount)
    {
        Parser echoRequestParser(new_ref<Context>(data, itemCount));
        ref_of<request>::type requestState = new_ref<request>(&echoRequestParser);

        echoRequestParser.SetInitialState(requestState.AsRef<State>());
        echoRequestParser.Run();
        
        auto requestNode = echoRequestParser.PopNode().template AsRef<RequestNode>();
        return requestNode->GetObject();
    }

}}}}//qor::components::protocols::echo