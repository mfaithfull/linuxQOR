// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ROLE_FEATURE_INTERFFACE
#define QOR_PP_H_ROLE_FEATURE_INTERFFACE

#include "src/qor/memory/injection/typeidentity.h"

namespace qor{ namespace app {

    class IRole;

    //Base interface for features. All features are dervied from this so that Roles can manage them
    class IFeature
    {
        friend class Role;
    public:

        IFeature() = default;
        virtual ~IFeature() = default;

        virtual void Setup() {};
        virtual void Shutdown() {};

    protected:

        IRole* m_Role{ nullptr };

    private:
        IFeature(const IFeature&) = delete;
        IFeature(const IFeature&&) = delete;
        IFeature& operator = (const IFeature&) = delete;
    };

}}//qor::app

#endif//QOR_PP_H_ROLE_FEATURE_INTERFFACE
