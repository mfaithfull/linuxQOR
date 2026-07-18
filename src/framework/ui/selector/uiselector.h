// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_SELECTOR
#define QOR_PP_H_FRAMEWORK_UI_SELECTOR
 
#include <atomic>
#include <coroutine>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/instance/singleton.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/app/role/ifeature.h"

namespace qor{ bool qor_pp_module_interface(QOR_UISELECTOR) ImplementsUISelector();}

namespace qor { namespace ui{
  
    class qor_pp_module_interface(QOR_UISELECTOR) UISelector : public app::IFeature
    {
    public:

        UISelector();
        virtual ~UISelector();

		UISelector(UISelector&& other) = delete;
		UISelector(const UISelector& other) = delete;
		UISelector& operator=(UISelector&& other) = delete;
		UISelector& operator=(const UISelector& other) = delete;

        virtual void Setup();
        virtual void Shutdown();

        //provide toolkits and rendering backends
        //provide parameters and metrics of the actual desktop
        //provide widgets that map onto the tookit and renderingapi in use.
        //provide singleton objects for the GUI, such as window class registration
        //provide access to the per thread GUI singletons for the toolkit and the renderingapi in use
        //provide a universal system to connect UI interactions to application events.
    };
    } //ui

    qor_pp_declare_instancer_of(ui::UISelector, SingletonInstancer);
    qor_pp_declare_factory_of(ui::UISelector, ExternalFactory);                                                                    
    constexpr GUID UISelectorGUID = {0xB062DEE1, 0xD3A3, 0x4315, {0x8D, 0xD1, 0xC0, 0x73, 0x97, 0xBA, 0xD3, 0xDE}};
    qor_pp_declare_guid_of(ui::UISelector,UISelectorGUID);

}//qor

#endif//QOR_PP_H_FRAMEWORK_UI_SELECTOR
