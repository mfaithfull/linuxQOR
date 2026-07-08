// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCES_PATHRESOURCE
#define QOR_PP_H_FRAMEWORK_RESOURCES_PATHRESOURCE

#include <string>

#include "../resource.h"
#include "fileresource.h"
#include "src/framework/io/filesystem/path.h"

namespace qor{ namespace framework{ namespace res {

    class qor_pp_module_interface(QOR_RESOURCES) Path : public Resource
    {
    public:

        static const char* StaticType();

        Path(ResourceManager* manager, const qor::io::filesystem::Path& path, Resource* batchKey = nullptr) : Resource(manager, batchKey), m_path(path), m_monitor(false), m_recurse(false)
        {            
            Name();
        }
        
        virtual ~Path() = default;

        virtual const char* Type(); //type, maybe mime-type of the resource, matching this class
        virtual void Name();
        virtual void Locate();
        virtual void Claim();

        void AddContainedTypesHints(std::vector<const char*>& hints);        
        void MarkForMonitoring();
        void IncludeSubFolders();

    protected:

        const qor::io::filesystem::Path m_path;
        bool m_monitor;
        std::vector<const char*> m_contentHints;
        std::map<std::string, res::File> m_files;
        std::map<std::string, res::Path> m_paths;
        bool m_recurse;
        
    private:
        static const char* s_pathResourceType;// = "Path";        

        bool IsARequestedPath(const io::filesystem::Index& index);
        bool IsARequestedResource(const io::filesystem::Index& index);
    };

}}}//qor::framework::res

#endif//QOR_PP_H_FRAMEWORK_RESOURCES_PATHRESOURCE