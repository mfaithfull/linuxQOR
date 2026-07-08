// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/framework/io/filesystem/fileindex.h"
#include "src/framework/io/filesystem/folder.h"
#include "src/framework/app/role/getfeature.h"
#include "pathresource.h"
#include "../manager.h"

namespace qor{ namespace framework{ namespace res {

    const char* Path::s_pathResourceType = "Path resource type";

    const char* Path::StaticType(){ return s_pathResourceType;}

    const char* Path::Type()
    {
        return s_pathResourceType;
    }
    
    void Path::Name()
    {
        std::string pathData = m_path.ToString();
        //TODO:
        std::string uri = "path://<ip_address>/" + pathData;
        //To get a URI for the path
        //Firstly it's a path so prepend path://
        //Secondly it's a path local to this host so we need to add either the hostname or IP path://192.168.0.0/...
        m_URI = uri;
        Resource::Name();        
    }

    void Path::Locate()
    {
        io::filesystem::Index index(m_path.ToString(), "");
        if(index.Exists())
        {
            Resource::Locate();            
        }
        else
        {
            std::string expected = m_path.ToString();
            continuable("Expected resource {0} not present.", expected);
        }
    }

    void Path::AddContainedTypesHints(std::vector<const char*>& hints)
    {
        for(const char* hint : hints)
        {
            if(std::find(m_contentHints.begin(), m_contentHints.end(), hint) == m_contentHints.end())
            {
                m_contentHints.emplace_back(hint);
            }
        }
    }

    void Path::MarkForMonitoring()
    {
        m_monitor = true;
    }

    void Path::IncludeSubFolders()
    {
        m_recurse = true;
    }

    void Path::Claim()
    {        
        //If the path has been marked as a container for certain types, set off a task to scan the path for subsidiary resources
        auto threadPool = GetFeature<thread::ThreadPool>();
        qor::io::filesystem::Folder folder(m_path);
        ResourceManager* manager = m_manager;
        bool includeSubFolders = this->m_recurse;
        Resource* batch = this;

        threadPool->PostTask(
            [folder, manager, includeSubFolders, batch]()        
            {
                manager->BeginBatch(batch);
                folder.Enumerate(
                    [manager, includeSubFolders, batch](io::filesystem::Index& index)->bool
                    {
                        if(includeSubFolders && index.IsDirectory())
                        {
                            manager->AddPath(index.GetPath(), batch);
                        }
                        else if(index.IsRegularFile())
                        {
                            manager->AddFile(index, batch);
                        }
                        return true;
                    }
                );
                manager->EndBatch(batch);
            }
        );

        //If the path has been marked as monitor then set up file system monitoring
        if(m_monitor)
        {

        }
    }

    bool Path::IsARequestedPath(const io::filesystem::Index& index)
    {        
        if(m_recurse && index.IsDirectory())
        {
            return true;
        }
        return false;
    }

    bool Path::IsARequestedResource(const io::filesystem::Index& index)
    {        
        return true;
    }

}}}//qor::framework::res