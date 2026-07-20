// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_SERIALCONSOLE_COMPOSITOR
#define QOR_PP_H_FRAMEWORK_UI_SERIALCONSOLE_COMPOSITOR

#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"
#include "../toolkit/consoleview.h"
#include "../toolkit/consoleviewmodel.h"
#include "../common/size.h"
#include "../common/position.h"

namespace qor { namespace ui { namespace sercon {

    class qor_pp_module_interface(QOR_SERIALCONSOLE) Compositor
    {
    public:

        Compositor();
        ~Compositor();

        void Compose(ref_of<ConsoleViewModel>::type model, ref_of<ConsoleView>::type view);
        void Compose(Position position, Size size, ref_of<ConsoleView>::type viewSource, ref_of<ConsoleView>::type viewTarget);
    };

}}}//qor::ui::sercon

#endif//QOR_PP_H_FRAMEWORK_UI_SERIALCONSOLE_COMPOSITOR
