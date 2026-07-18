// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_FRAMEWORK_CONSOLE
#define QOR_PP_H_COMPONENTS_FRAMEWORK_CONSOLE

#include "src/framework/app/role/ifeature.h"
#include "iconsole.h"

namespace qor
{
    bool qor_pp_import ImplementsIConsole();//All libraries providing an implementation of IConsole also need to export this function so that the linker can find them
}

namespace qor { namespace components {

        class qor_pp_module_interface(QOR_CONSOLE) Console : public app::IFeature
        {
        public:

            Console();
            ~Console() noexcept = default;

            virtual void Setup() {}
            virtual void Shutdown() {}

            string_t ReadLine();
            void WriteLine(const string_t& line);
            char_t ReadChar();
            void WriteChar(char_t c);

        private:

            ref_of<IConsole>::type m_pimpl;
        };

    }//qor::components

    constexpr GUID ConsoleGUID = { 0xc4277aad, 0x3a81, 0x4f19, {0xba, 0x77, 0x2d, 0xaa, 0xa1, 0x1a, 0x9f, 0x0a} };
    qor_pp_declare_guid_of(components::Console, ConsoleGUID);

}//qor

#endif//QOR_PP_H_COMPONENTS_FRAMEWORK_CONSOLE
