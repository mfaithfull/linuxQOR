// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_FRAMEWORK_CONSOLE_INTERFACE
#define QOR_PP_H_COMPONENTS_FRAMEWORK_CONSOLE_INTERFACE

#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/instance/singleton.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "src/qor/memory/reference/reference.h"

namespace qor { namespace components {

        class qor_pp_module_interface(QOR_CONSOLE) IConsole
        {
        public:

            IConsole() = default;
            virtual ~IConsole() noexcept = default;

            virtual void Setup() {}
            virtual void Shutdown() {}

            virtual string_t ReadLine()
            {
                return string_t();
            }

            virtual void WriteLine(const string_t& /*line*/){}
            
            virtual char_t ReadChar()
            {
                return 0;
            }

            virtual void WriteChar(char_t /*c*/) {}

        };

    }//qor::components

    qor_pp_declare_instancer_of(components::IConsole, SingletonInstancer);
    qor_pp_declare_factory_of(components::IConsole, ExternalFactory);
    constexpr GUID IConsoleGUID = { 0xcbcea0af, 0x9f81, 0x4644, { 0x88, 0x5d, 0x61, 0x80, 0x8a, 0x25, 0x48, 0x56} };
    qor_pp_declare_guid_of(components::IConsole, IConsoleGUID);

}//qor

#endif//QOR_PP_H_COMPONENTS_FRAMEWORK_CONSOLE_INTERFACE
