// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/error/error.h"
#include "src/framework/io/filesystem/fileindex.h"
#include "src/framework/io/filesystem/folder.h"
#include "fileresource.h"
#include "../manager.h"

namespace qor{ namespace framework{ namespace res {

    const char* File::s_fileResourceType = "File resource type";

    const char* File::StaticType(){ return s_fileResourceType;}

    const char* File::Type()
    {
        return s_fileResourceType;
    }
    
    void File::Name()
    {
        std::string path = m_index.GetPath().ToString();
        std::string name = m_index.ToString();
        //std::string pathData = m_path.ToString();        
        //std::string uri = "file://<ip_address>/" + path + fileName
        //To get a URI for the path
        //Firstly it's a path so prepend path://
        //Secondly it's a path local to this host so we need to add either the hostname or IP path://192.168.0.0/...
        m_URI = "file://<ip-address>/" + name;   
        Resource::Name();     
    }

    void File::Locate()
    {        
        if(m_index.Exists())
        {
            Resource::Locate();
        }
        else
        {            
            continuable("Expected file resource {0} not present.", m_URI);
        }
    }

    void File::MarkForMonitoring()
    {
        m_monitor = true;
    }

    //Allow content hints to be set so we know what might be in the file to be claimed

    void File::Claim()
    {        
        //Claim the resources in the file or claim the whole file as a resource is it is opaque
        std::string type = GetContentGuess();

        if(type == "unknown")
        {
            //treat file as a blob
        }
        if(type == "JSON")
        {
            m_manager->AddJSON(m_index);
            return;
        }
        if(type == "font")
        {
            m_manager->AddFont(m_index);
            return;
        }
        if(type == "bitmap")
        {
            //Create bitmap image resource from the file.
            return;
        }
        //If the file has been marked as monitor then set up file system monitoring
        if(m_monitor)
        {

        }
    }

    std::string File::GetContentGuess()
    {
        if(m_URI.ends_with(".txt"))
        {
            return "text";
        }
        if(m_URI.ends_with(".ttf"))
        {
            return "font";
        }
        if(m_URI.ends_with(".json"))
        {
            return "JSON";
        }

        return "unknown";
    }

}}}//qor::framework::res