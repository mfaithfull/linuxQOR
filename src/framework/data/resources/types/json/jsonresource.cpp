// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/framework/io/filesystem/fileindex.h"
#include "src/framework/io/filesystem/folder.h"
#include "jsonresource.h"
#include "reader.h"
#include "src/components/io/pipeline/connectors/fileconnector/fileconnector.h"

namespace qor{ namespace framework{ namespace res {

    const char* JSON::s_jsonResourceType = "JSON resource type";

    const char* JSON::StaticType(){ return s_jsonResourceType;}

    const char* JSON::Type()
    {
        return s_jsonResourceType;
    }
    
    void JSON::Name()
    {        
        std::string name = m_index.ToString();
        m_URI = "json://<ip-address>/" + name;
        Resource::Name();
    }

    void JSON::Locate()
    {        
        //Read contents of file and parse
        JSONReader reader;
        
        m_object = reader(
            qor::io::components::FileConnector(
                m_index,
                reader.Buffer(),
                qor::io::OpenFor::ReadOnly,
                qor::io::WithFlags::None
            )
        );  
        if(m_object.IsNotNull())
        {
            Resource::Locate();
        }
        return;
    }

    void JSON::Claim()
    {        
        //Claim the JSON object resulting from the parse
        Resource::Claim();
    }

    qor::ref_of<qor::components::model::json::Object>::type JSON::GetObject()
    {
        return m_object;
    }

}}}//qor::framework::res