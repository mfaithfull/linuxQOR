// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "filter.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/informative.h"

using namespace qor;
using namespace qor::components;
using namespace qor::components::parser;
using namespace qor::workflow;

namespace qor { namespace components { namespace protocols { namespace http {

    HTTPFilter::HTTPFilter(size_t itemCount) : qor::pipeline::InlineFilter<qor::byte>(itemCount){ }

    HTTPFilter::~HTTPFilter() = default;

    void HTTPFilter::Filter(byte* space, byte* data, size_t& itemCount, size_t& writeCount)
    {
        ref_of<HTTPRequest>::type request = Parse(data, itemCount);
        
        if(request.IsNull())
        {
            //TODO: return a Bad Request Response
            return;
        }

        ref_of<HTTPResponse>::type response = new_ref<HTTPResponse>();
        
        //TODO: Fill out Response        

        response::Generator responseGenerator(new_ref<parser::Context>(space, writeCount));
        responseGenerator.SetResponse(response);
        responseGenerator.Run();
        writeCount = responseGenerator.GetContext()->GetPosition();
    }

    ref_of<HTTPRequest>::type HTTPFilter::Parse(byte* data, size_t& itemCount)
    {
        Parser HTTPRequestParser(new_ref<Context>(data, itemCount));
        ref_of<request>::type requestState = new_ref<request>(&HTTPRequestParser);

        HTTPRequestParser.SetInitialState(requestState.AsRef<State>());
        HTTPRequestParser.Parse();
        
        auto requestNode = HTTPRequestParser.PopNode().template AsRef<RequestNode>();
        if(requestNode.IsNull())
        {
            return ref_of<HTTPRequest>::type();
        }
        return requestNode->GetObject();
    }

}}}}//qor::components::protocols::http