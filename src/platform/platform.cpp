// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "platform.h"

namespace qor{ 
    
    qor_pp_module_interface(QOR_PLATFORM) ref_of<platform::Platform>::type ThePlatform()
    {
        return new_ref<platform::Platform>();
    }

    namespace platform{

    Platform::Platform() : setupCompleted(false) { }

    Platform::~Platform() = default;
    
    void Platform::Setup()
    {
        if(!setupCompleted)
        {
            for( auto it : m_mapSubsystems)
            {
                it.second->Setup();
            }    
        }
        setupCompleted = true;
    }

    void Platform::Shutdown()
    {   //TODO: consider shutting down the sub systems in reverse order as we do with features
        for( auto it : m_mapSubsystems)
        {
            it.second->Shutdown();            
        }
        m_mapSubsystems.clear();
    }

    void Platform::AddSubsystem( const GUID* id, ref_of<ISubsystem>::type subsystem)
    {
        m_mapSubsystems.insert(std::make_pair(*id, subsystem));
        if(setupCompleted)
        {
            subsystem->Setup(); //Late initialisation of sub system
        }
    }

    ref_of<ISubsystem>::type Platform::GetSubsystem(const GUID* id)
    {
        ref_of<ISubsystem>::type result;
        auto it = m_mapSubsystems.find(*id);
        if( it != m_mapSubsystems.end())
        {
            result = it->second;
        }
        return result;
    }

}}//qor::platform
