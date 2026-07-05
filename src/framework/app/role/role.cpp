// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "role.h"
#include "src/platform/platform.h"
#include "src/qor/flyers/error/error.h"

namespace qor{ namespace app {

    Role::Role() { }

    Role::Role(const Role& src)
    {
        m_mapFeatures = src.m_mapFeatures;
    }

    Role& Role::operator = (const Role& src)
    {
        if(&src != this)
        {
            m_mapFeatures = src.m_mapFeatures;
        }
        return *this;
    }

    Role::~Role() { }

    void TrySetup(ref_of<IFeature>::type feature)
    {
        try{
            feature->Setup();
        }
        catch(const qor::Serious& err)
        {
            serious("Feature setup failed with error {0}.", err.HasWhat() ? err.what().Content() : "");
        }
        catch(const std::exception& e)
        {
            std::string s(e.what());
            fatal("Feature setup failed with exception {0}.", s);
        }
        catch(...)
        {
            fatal("Feature setup failed. Terminating.");
        }
    }

    void TryShutdown(ref_of<IFeature>::type feature)
    {
        try
        {
            feature->Shutdown();
        }
        catch(const qor::Serious& err)
        {
            warning("Feature shutdown failed with error {0}.", err.HasWhat() ? err.what().Content() : "");
        }
        catch(const std::exception& e)
        {
            std::string s(e.what());
            warning("Feature shutdown failed with exception {0}.", s);
        }
        catch(...)
        {
            warning("Feature shutdown failed with an unhandled exception.");
        }
    }

    void Role::Setup()
    {
        ThePlatform(qor_shared)->Setup();
        for(auto feature: m_mapFeatures)
        {
            TrySetup(feature.second);
        }
    }

    void Role::Shutdown()
    {
        for (auto feature = m_mapFeatures.rbegin(); feature != m_mapFeatures.rend(); ++feature) 
        {
            TryShutdown(feature->second);
        }
        m_mapFeatures.clear();
        ThePlatform(qor_shared)->Shutdown();
    }

    ref_of<IFeature>::type Role::GetFeature(const GUID* id)
    {
        auto it = m_mapFeatures.find(*id);
        if( it != m_mapFeatures.end())
        {
            return (*it).second.Clone();
        }
        continuable("Feature with GUID {0} not found. A null refence will be returned.");
        return ref_of<IFeature>::type(nullptr);
    }

    void Role::AddFeature(const GUID* id, ref_of<IFeature>::type feature)
    {
        if(*id == null_guid)
        {
            continuable("Attempting to add a feature without a GUID. Did you forget to qor_pp_declare_guid_of(`feature`,`guid`); ?");
        }
        if(feature.IsNotNull())
        {
            feature->m_Role = this;
            m_mapFeatures.insert(std::make_pair(*id, feature));
        }
        else
        {
            continuable("Cannot add null feature to role.");
        }
    }

}}//qor::app
