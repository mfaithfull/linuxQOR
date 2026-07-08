// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCES_RESOURCE
#define QOR_PP_H_FRAMEWORK_RESOURCES_RESOURCE

#include <string>

#include "src/framework/event/signals/signal.h"

namespace qor{ namespace framework{

    enum ResourceStatus
    {
        Unknown,        //No resource of this name is known. Default initial state until a URI is assigned
        Named,          //Resource has a URI but may not have any other existance. Can still be used as an agreed atom but that's all.
        Located,        //Resource confirmed to exist, file, path, etc is a real thing as least for now.
        Claimed,        //We have opened the file/thing and if possible locked it/assigned it to a ResourceHub instance
        Available,      //The resource has been loaded, parsed and is immediately available as a Model
        Removed,        //The resource has gone away. Notification only. 
    };

    class qor_pp_module_interface(QOR_RESOURCES) ResourceManager;

    //Managable Resource base class
    class qor_pp_module_interface(QOR_RESOURCES) Resource : public SignalBase
    {
    public:

        Resource(ResourceManager* manager, Resource* batchKey = nullptr);
        virtual ~Resource();
        ResourceManager* Manager();
        virtual const char* Type(); //type, maybe mime-type of the resource, matching this class
        std::string URI();

        virtual void Name();
        virtual void Locate();
        virtual void Claim();
        
        qor_pp_signal_func StatusEvent(Resource* res, ResourceStatus status);

    protected:
        Resource* m_batchKey;
        ResourceManager* m_manager;
        ResourceStatus m_status;
        std::string m_URI;  //unique identifier for the resource        

    private:
        static const char* s_baseResourceType;
    };

}}//qor::framework

#endif//QOR_PP_H_FRAMEWORK_RESOURCES_RESOURCE