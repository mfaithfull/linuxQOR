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
using namespace qor::fastflow;

namespace qor { namespace components { namespace protocols { namespace echo {

    //Server side protocol filter
    
    EchoServiceFilter::EchoServiceFilter() : pipeline::InlineFilter<byte>(512), m_sourceContext(m_sourceBuffer), m_requestParser(&m_sourceContext)
    { 
        ref_of<qor::components::protocols::echo::request>::type requestState = 
        new_ref<qor::components::protocols::echo::request>(&m_requestParser);
        m_requestParser.SetInitialStep(requestState);
    }
    
    EchoServiceFilter::~EchoServiceFilter() = default;

    size_t EchoServiceFilter::WriteAcknowledge(size_t& itemCount)
    {            
        m_sourceBuffer.WriteAcknowledge(itemCount);
        
        int parseResult = m_requestParser.FinalParse();

        if(m_requestParser.IsComplete())
        {
            auto requestNode = m_requestParser.PopNode().template AsRef<RequestNode>();
            ref_of<EchoRequest>::type Request = requestNode->GetObject();
            HandleRequest(Request);
        }

        return m_sourceBuffer.WriteCapacity();
    }

    void EchoServiceFilter::HandleRequest(ref_of<EchoRequest>::type Request)
    {
        //Write out the response to the client
        std::string data = Request->GetValue(); 
        size_t itemCount = data.size();
        byte* space = m_sinkBuffer.WriteRequest(itemCount);
        if(itemCount > 0)
        {
            memcpy(space, data.data(), itemCount);
            m_sinkBuffer.WriteAcknowledge(itemCount);
        }
        
        ref_of<request>::type requestState = new_ref<request>(&m_requestParser);
        m_requestParser.SetInitialStep(requestState);
    }

}}}}//qor::components::protocols::echo