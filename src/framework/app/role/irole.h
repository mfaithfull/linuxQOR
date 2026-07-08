// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ROLE_INTERFACE
#define QOR_PP_H_ROLE_INTERFACE

#include "ifeature.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/memory/reference/newref.h"

namespace qor{ namespace app {

    //Base interface for Roles. All Roles are derived from this so that the Application can have any Role
    class IRole
    {
    public:

        typedef ref_of<IRole>::type ref;

        IRole() = default;
        virtual ~IRole() = default;
        virtual void Setup() = 0;
        virtual void Shutdown() = 0;
        virtual void AddFeature( const GUID* id, ref_of<IFeature>::type feature) = 0;
        virtual ref_of<IFeature>::type GetFeature(const GUID* id) = 0;

        template< class TFeature >
        void AddFeature()
        {
            AddFeature(guid_of<TFeature>::guid(), new_ref<TFeature>().template AsRef<IFeature>());
        }

        template< class TFeature, typename feature_config_func>
        void AddFeature(feature_config_func&& configure)
        {
			ref_of<IFeature>::type feature = new_ref<TFeature>();
            AddFeature(guid_of<TFeature>::guid(), feature.template AsRef<IFeature>());
			configure(feature);            
        }

        template< class TFeature >
        ref_of<TFeature>::type GetFeature()
        {
            return GetFeature(guid_of<TFeature>::guid()).template AsRef<TFeature>();
        }

    };

}}//qor::app

#endif//QOR_PP_H_ROLE_INTERFACE
