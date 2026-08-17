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

    EchoResponseFilter::EchoResponseFilter() :  qor::pipeline::InlineFilter<qor::byte>(512), m_sourceContext(m_sourceBuffer), m_responseParser(&m_sourceContext)
    {
        ref_of<response>::type responseState = new_ref<response>(&m_responseParser);
        m_responseParser.SetInitialStep(responseState);
    }
    
    EchoResponseFilter::~EchoResponseFilter() = default;

    size_t EchoResponseFilter::WriteAcknowledge(size_t& itemCount)
    {            
        m_sourceBuffer.WriteAcknowledge(itemCount);
        
        int parseResult = m_responseParser.FinalParse();

        if(m_responseParser.IsComplete())
        {
            auto responseNode = m_responseParser.PopNode().template AsRef<ResponseNode>();            
            HandleResponse(responseNode->GetObject());
        }

        return m_sourceBuffer.WriteCapacity();
    }

    void EchoResponseFilter::HandleResponse(ref_of<EchoResponse>::type Response)
    {
        std::string output = Response->GetValue();

        if(output == "quit")
        {
            continuable("user wants to quit");
        }

        size_t outputSize = output.size();
        byte* space = m_sinkBuffer.WriteRequest(outputSize);
        if(outputSize > 0)
        {
            memcpy(space, output.data(), outputSize);
            m_sinkBuffer.WriteAcknowledge(outputSize);
        }

        ref_of<response>::type responseState = new_ref<response>(&m_responseParser);
        m_responseParser.SetInitialStep(responseState);

    }
    
}}}}//qor::components::protocols::echo
