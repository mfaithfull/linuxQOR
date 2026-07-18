// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_CONSOLE_COMPOSITOR
#define QOR_PP_H_COMPONENTS_CONSOLE_COMPOSITOR

#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"
#include "consoleview.h"
#include "consoleviewmodel.h"
#include "size.h"
#include "position.h"

namespace qor { namespace components {

    class qor_pp_module_interface(QOR_CONSOLE) ConsoleCompositor
    {
    public:

        ConsoleCompositor();
        ~ConsoleCompositor();

        void Compose(ref_of<ConsoleViewModel>::type model, ref_of<ConsoleView>::type view);
        void Compose(Position position, Size size, ref_of<ConsoleView>::type viewSource, ref_of<ConsoleView>::type viewTarget);
    };

}}//qor::components

#endif//QOR_PP_H_COMPONENTS_CONSOLE_COMPOSITOR
