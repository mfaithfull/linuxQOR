// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0.

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_IMAINWINDOWIMPL
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_IMAINWINDOWIMPL

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/newref.h"
#include "iwindowimpl.h"

namespace qor{ namespace ui {
    
    class qor_pp_module_interface(QOR_DESKTOPUI) IMainWindowImpl : public IWindowImpl
    {
    public:

        IMainWindowImpl() = default;
        IMainWindowImpl(const std::wstring& /*title*/){ }
        virtual ~IMainWindowImpl() = default;

    };
    }//ui

    qor_pp_declare_factory_of(ui::IMainWindowImpl, ExternalFactory);                                            
    constexpr GUID IMainWindowImplGUID = {0xD2C42FF8, 0x1474, 0x4CA6, {0x83, 0xB5, 0xAE, 0x63, 0xE3, 0xEC, 0x9B, 0xF0}};
    qor_pp_declare_guid_of(ui::IMainWindowImpl,IMainWindowImplGUID);

}//qor

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_IMAINWINDOWIMPL

