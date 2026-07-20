// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_TERMINAL
#define QOR_PP_H_FRAMEWORK_UI_TERMINAL

#include "src/qor/essentials/datastructures/guid.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "src/framework/app/role/ifeature.h"
#include "toolkit/toolkit.h"

namespace qor{

    bool qor_pp_module_interface(QOR_TERMINAL) ImplementsTerminal();

    namespace ui {
        
        class qor_pp_module_interface(QOR_TERMINAL) Terminal : public app::IFeature
        {
        public:
            Terminal();
            ~Terminal() noexcept;
            Terminal(Terminal&& other) = delete;
            Terminal(const Terminal& other) = delete;
            Terminal& operator=(Terminal&& other) = delete;
            Terminal& operator=(const Terminal& other)= delete;

            virtual void Setup();
            virtual void Shutdown();

            //ref_of<ToolKit>::type GetToolkit();
        };

    }//ui

    qor_pp_declare_factory_of(ui::Terminal, ExternalFactory)    
    constexpr GUID TerminalGUID = {0x64BF6A71, 0x8EC4, 0x48A9, {0x85, 0xEF, 0x14, 0x03, 0x0F, 0x5C, 0x01, 0xB6} };
    qor_pp_declare_guid_of(ui::Terminal,TerminalGUID);

}//qor
#endif//QOR_PP_H_FRAMEWORK_UI_TERMINAL