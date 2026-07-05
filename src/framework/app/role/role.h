// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ROLE
#define QOR_PP_H_ROLE

#include <map>
#include "irole.h"

namespace qor{ namespace app {

    //Base Role implementation. This fully functional feature container can be extended if required.
    class qor_pp_module_interface(QOR_ROLE) Role : public IRole
    {
    public:

        Role();
        Role(const Role&);
        virtual ~Role();
        Role& operator = (const Role& src);
        qor_pp_noinline virtual void Setup();
        qor_pp_noinline virtual void Shutdown();
        virtual void AddFeature( const GUID* id, ref_of<IFeature>::type feature) override;
        virtual ref_of<IFeature>::type GetFeature(const GUID* id) override;

    protected:

        std::map<GUID, ref_of<IFeature>::type > m_mapFeatures;
    };

}}//qor

#endif//QOR_PP_H_ROLE
