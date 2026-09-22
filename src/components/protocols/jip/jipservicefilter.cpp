// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "jipservicefilter.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/informative.h"

using namespace qor;
using namespace qor::components;
using namespace qor::data;
using namespace qor::data::parser;
using namespace qor::fastflow;

namespace qor { namespace components { namespace protocols { namespace jip {

    //Server side protocol filter
    
    JipServiceFilter::JipServiceFilter() : pipeline::InlineFilter<byte>(1), m_sourceContext(m_sourceBuffer), m_requestParser(&m_sourceContext)
    { 
        ref_of<qor::components::protocols::jip::request>::type requestState = 
        new_ref<qor::components::protocols::jip::request>(&m_requestParser);
        m_requestParser.SetInitialStep(requestState);
    }
    
    JipServiceFilter::~JipServiceFilter() = default;

    size_t JipServiceFilter::WriteAcknowledge(size_t& itemCount)
    {            
        m_sourceBuffer.WriteAcknowledge(itemCount);
        
        int parseResult = m_requestParser.FinalParse();

        if(m_requestParser.IsComplete())
        {
            auto requestNode = m_requestParser.PopNode().template AsRef<RequestNode>();
            ref_of<JipRequest>::type Request = requestNode->GetObject();
            HandleRequest(Request);
        }

        return m_sourceBuffer.WriteCapacity();
    }

    void JipServiceFilter::HandleRequest(ref_of<JipRequest>::type Request)
    {
        //Write out the response to the client
        //TODO: Replace this with JSON to pipeline generator
        std::string data = Request->GetValue(); 
        size_t itemCount = data.size();
        byte* space = m_sinkBuffer.WriteRequest(itemCount);
        if(itemCount > 0)
        {
            memcpy(space, data.data(), itemCount);
            m_sinkBuffer.WriteAcknowledge(itemCount);
        }
        
        //Reset Parser for next request.
        ref_of<request>::type requestState = new_ref<request>(&m_requestParser);
        m_requestParser.SetInitialStep(requestState);
    }

}}}}//qor::components::protocols::jip