// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_PLATFORM
#define QOR_PP_H_PLATFORM_PLATFORM

#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/memory/reference/newref.h"
#include "isubsystem.h"

/*The Platform class represents the OS at runtime, it's capabilities services, 
features, configuration and settings including the devices it presents.
This works very much like qor::app::Role but for OS subsystems rather than
application features*/

namespace qor{ namespace platform{

    class Platform final
    {
    public:

        qor_pp_module_interface(QOR_PLATFORM) Platform();     
        qor_pp_module_interface(QOR_PLATFORM) ~Platform();
        qor_pp_module_interface(QOR_PLATFORM) void Setup();
        qor_pp_module_interface(QOR_PLATFORM) void Shutdown();
        
        template< class T >
        void AddSubsystem()
        {
            AddSubsystem(guid_of<T>::guid(), new_ref<T>().template AsRef<ISubsystem>());
        }

        qor_pp_module_interface(QOR_PLATFORM) void AddSubsystem( const GUID* id, ref_of<ISubsystem>::type subsystem);

        template< class T >
        ref_of<T>::type GetSubsystem()
        {
            return GetSubsystem(guid_of<T>::guid()).template AsRef<T>();
        }

        qor_pp_module_interface(QOR_PLATFORM) ref_of<ISubsystem>::type GetSubsystem(const GUID* id);

    private:

        std::map<GUID, ref_of<ISubsystem>::type > m_mapSubsystems;
        bool setupCompleted;
    };

    }//qor::platform

    qor_pp_declare_instancer_of(platform::Platform, SingletonInstancer);
    qor_pp_declare_sync_of(platform::Platform, RecursiveMutex);

    qor_pp_module_interface(QOR_PLATFORM) ref_of<platform::Platform>::type ThePlatform();
}//qor

#endif//QOR_PP_H_PLATFORM_PLATFORM
