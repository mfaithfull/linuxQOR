// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCES_JSONRESOURCE
#define QOR_PP_H_FRAMEWORK_RESOURCES_JSONRESOURCE

#include <string>

#include "../../resource.h"
#include "src/framework/io/filesystem/path.h"
#include "src/framework/io/filesystem/fileindex.h"
#include "src/components/protocols/json/model/object.h"

namespace qor{ namespace framework{ namespace res {

    class qor_pp_module_interface(QOR_RESOURCES) JSON : public Resource
    {
    public:

        static const char* StaticType();

        JSON(ResourceManager* manager, const io::filesystem::Index& index, Resource* batchKey = nullptr) : Resource(manager, batchKey), m_index(index)
        {            
            Name();
        }
        
        virtual ~JSON() = default;

        virtual const char* Type();
        virtual void Name();
        virtual void Locate();
        virtual void Claim();
        
        ref_of<components::model::json::Object>::type GetObject();
        
    protected:

        const io::filesystem::Index m_index;
        ref_of<components::model::json::Object>::type m_object;
        
    private:
        static const char* s_jsonResourceType;
        
    };

}}}//qor::framework::res

#endif//QOR_PP_H_FRAMEWORK_RESOURCES_JSONRESOURCE