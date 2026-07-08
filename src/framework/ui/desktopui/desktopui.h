// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI
#define QOR_PP_H_FRAMEWORK_DESKTOPUI
 
#include <atomic>
#include <coroutine>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/instance/singleton.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/app/role/ifeature.h"

namespace qor{ bool qor_pp_module_interface(QOR_DESKTOPUI) ImplementsDesktopUI();}

namespace qor { namespace ui{
  
    class qor_pp_module_interface(QOR_DESKTOPUI) DesktopUI : public app::IFeature
    {
    public:

        DesktopUI();
        virtual ~DesktopUI();

		DesktopUI(DesktopUI&& other) = delete;
		DesktopUI(const DesktopUI& other) = delete;
		DesktopUI& operator=(DesktopUI&& other) = delete;
		DesktopUI& operator=(const DesktopUI& other) = delete;

        virtual void Setup();
        virtual void Shutdown();

        virtual int Run();
        //provide toolkits and rendering backends
        //provide parameters and metrics of the actual desktop
        //provide widgets that map onto the tookit and renderingapi in use.
        //provide singleton objects for the GUI, such as window class registration
        //provide access to the per thread GUI singletons for the toolkit and the renderingapi in use
        //provide a universal system to connect UI interactions to application events.
    };
    } //ui

    qor_pp_declare_instancer_of(ui::DesktopUI, SingletonInstancer);
    qor_pp_declare_factory_of(ui::DesktopUI, ExternalFactory);                                
    constexpr GUID DesktopUIGUID = {0x76426824, 0x4A0B, 0x4B18, {0x8F, 0xFC, 0x15, 0x3A, 0x78, 0xED, 0xFE, 0x03}};
    qor_pp_declare_guid_of(ui::DesktopUI,DesktopUIGUID);

}//qor

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI
