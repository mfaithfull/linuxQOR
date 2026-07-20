// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_UI_FRAMEWORK_SERIALCONSOLE
#define QOR_PP_H_UI_FRAMEWORK_SERIALCONSOLE

#include "src/qor/memory/reference/newref.h"
#include "src/framework/app/role/ifeature.h"
#include "renderer/renderer.h"

namespace qor
{
    bool qor_pp_module_interface(QOR_CONSOLE) ImplementsSerialConsole();
}

namespace qor { namespace ui {

        class qor_pp_module_interface(QOR_CONSOLE) SerialConsole : public app::IFeature
        {
        public:

            SerialConsole();
            ~SerialConsole() noexcept;
            virtual void Setup();
            virtual void Shutdown();
            virtual string_t ReadLine();
            virtual void WriteLine(const string_t& line);
            virtual char_t ReadChar();
            virtual void WriteChar(char_t c);
            ref_of<sercon::Renderer>::type GetRenderer();
            ref_of<sercon::TokenBuffer>::type CreateSurface();

        private:

            ref_of<sercon::Renderer>::type m_renderer;
        };

    }//qor::ui
                                       
    qor_pp_declare_factory_of(ui::SerialConsole, ExternalFactory)  
    constexpr GUID SerialConsoleGUID = { 0xB218B8D2, 0x5417, 0x4B9E, {0x8A, 0xC5, 0x2B, 0x10, 0x59, 0xC2, 0x6F, 0xC0} };
    qor_pp_declare_guid_of(ui::SerialConsole, SerialConsoleGUID);

}//qor

#endif//QOR_PP_H_UI_FRAMEWORK_SERIALCONSOLE
