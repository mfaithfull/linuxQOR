// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCES_FILERESOURCE
#define QOR_PP_H_FRAMEWORK_RESOURCES_FILERESOURCE

#include <string>

#include "../resource.h"
#include "src/framework/io/filesystem/path.h"
#include "src/framework/io/filesystem/fileindex.h"

namespace qor{ namespace framework{ namespace res {

    class qor_pp_module_interface(QOR_RESOURCES) File : public Resource
    {
    public:

        static const char* StaticType();

        File(ResourceManager* manager, const io::filesystem::Index& index, Resource* batchKey = nullptr) : Resource(manager, batchKey), m_index(index), m_monitor(false)
        {            
            Name();
        }
        
        virtual ~File() = default;

        virtual const char* Type();
        virtual void Name();
        virtual void Locate();
        virtual void Claim();
        
        void MarkForMonitoring();

    protected:

        const io::filesystem::Index m_index;
        bool m_monitor;
        
    private:
        static const char* s_fileResourceType;

        std::string GetContentGuess();
    };

}}}//qor::framework::res

#endif//QOR_PP_H_FRAMEWORK_RESOURCES_FILERESOURCE