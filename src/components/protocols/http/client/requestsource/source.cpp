// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "source.h"
#include "src/framework/data/pipeline/sink.h"

namespace qor { namespace components { namespace protocols { namespace http {

    HTTPSource::HTTPSource() :
        m_context(new_ref<Context>()), m_generator(m_context){ }

    void HTTPSource::SetRequest(ref_of<HTTPRequest>::type request)
    {        
        m_generator.SetRequest(request);
    }
    
    size_t HTTPSource::ReadBytes(byte* space, size_t bytesToRead)
    {
        m_context->SetSpace(space, bytesToRead);                        
        m_generator.Run();
        return m_context->GetPosition();        
    }

}}}}//qor::components::protocols::http
