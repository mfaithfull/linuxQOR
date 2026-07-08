// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCE_CLAIMER
#define QOR_PP_H_FRAMEWORK_RESOURCE_CLAIMER

#include "resourcehub.h"
#include "src/framework/event/delegate/bindmember.h"

namespace qor{ namespace framework{

    //Responds to resource updates with the purpose of claiming a specific type of resource on a specific path
    //Can be waited on to synchronously claim a resource before proceeding
    template<class ResourceType>
    class Claimer
    {
    public:

        Claimer(ref_of<ResourceHub>::type hub, io::filesystem::Path& path)
        {
            m_resource = nullptr;
            hub->SubscribeForAllByPath(path.ToString(), bindMemberFunction(&Claimer<ResourceType>::Callback, this));
            hub->AddPath(path);
        }

        Claimer(ref_of<ResourceHub>::type hub, io::filesystem::Index& file)
        {
            m_resource = nullptr;
            hub->SubscribeForAllByPath(file.ToString(), bindMemberFunction(&Claimer<ResourceType>::Callback, this));
            hub->AddFile(file);
        }

        //Gets called from ResourceHub on a pool thread
        bool Callback(Resource* res, ResourceStatus status)
        {
            if(res)
            {
                if(res->Type() == res::Path::StaticType() && status == Named)
                {
                    res->Locate();
                    return true;
                }
                if(res->Type() == res::Path::StaticType() && status == Located)
                {
                    res::Path* path = dynamic_cast<res::Path*>(res);
                    if(path)
                    {
                        path->IncludeSubFolders();
                        path->Claim();
                    }
                    return true;
                }

                if(res->Type() == res::File::StaticType() && status == Named)
                {
                    res->Locate();
                    return true;
                }

                if(res->Type() == res::File::StaticType() && status == Located)
                {
                    res->Claim();
                    return true;
                }

                if(res->Type() == ResourceType::StaticType() && status == Named)
                {
                    res->Locate();
                    return true;
                }

                if(res->Type() == ResourceType::StaticType() && status == Located)
                {
                    res->Claim();
                    return true;
                }

                if(res->Type() == ResourceType::StaticType() && status == Claimed)
                {
                    m_resource = dynamic_cast<ResourceType*>(res);
                    if(m_resource)
                    {
                        std::lock_guard<std::mutex> lock(m_mutex);
                        m_cond.notify_one();
                    }
                    return false;
                }

            }
            return true;
        }

        ResourceType* GetResouce()
        {
            return m_resource;
        }

        void WaitForResource(ResourceType*& res)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cond.wait(lock);
            res = m_resource;
        }

    private:
        std::mutex m_mutex;
        std::condition_variable m_cond;
        ResourceType* m_resource;

    };

}}//qor::framework

#endif//QOR_PP_H_FRAMEWORK_RESOURCE_CLAIMER