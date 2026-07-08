// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "resource.h"
#include "manager.h"

namespace qor{ namespace framework{

    const char* Resource::s_baseResourceType = "Base resource type";

    Resource::Resource(ResourceManager* manager, Resource* batchKey) : m_status(Unknown), m_manager(manager), m_batchKey(batchKey)
    {
        qor::connect(*this, &Resource::StatusEvent, *m_manager, &ResourceManager::ReceiveResourceUpdate, qor::ConnectionKind::QueuedConnection);
    }

    Resource::~Resource()
    {        
        qor::disconnect(*this, &Resource::StatusEvent, *m_manager, &ResourceManager::ReceiveResourceUpdate);
    }

    ResourceManager* Resource::Manager()
    {
        return m_manager;
    }
    
    const char* Resource::Type()
    {
        return s_baseResourceType;
    }

    std::string Resource::URI()
    {
        return m_URI;
    }

    void Resource::Name()
    {
        //override to set the URI for the resource
        //Always call this base implementation at the end of the overriden implementation
        m_status = Named;
        StatusEvent(this, m_status);
    }

    void Resource::Locate()
    {
        //Override to verify the presence of the resource
        //Always call this base implementation at the end of the overrriden implementation
        m_status = Located;
        StatusEvent(this, m_status);
    }

    void Resource::Claim()
    {
        //Override to grab the resource, parse it and discover any sub resources
        //Always call this base implementation at the end of the overriden implementation
        m_status = Claimed;
        StatusEvent(this, m_status);
    }

    qor_pp_signal_func Resource::StatusEvent(Resource* res, ResourceStatus status)
    {
        qor_pp_signal(StatusEvent, res, status);
    }

}}//qor::framework