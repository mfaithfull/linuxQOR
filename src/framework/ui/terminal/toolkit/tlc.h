// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_TERMINAL_TOOKIT_TOPLEVELCONTAINER
#define QOR_PP_H_FRAMEWORK_UI_TERMINAL_TOOKIT_TOPLEVELCONTAINER

#include "src/framework/ui/widgets/tlc.h"

namespace qor{ namespace ui { namespace widgets { namespace tty {
    
    class qor_pp_module_interface(QOR_TERMINAL) TopLevelContainer : public widgets::TopLevelContainer
    {
    public:
        
        TopLevelContainer();
        virtual ~TopLevelContainer();
    };

}}}}//qor::ui::widgets::tty

#endif//QOR_PP_H_FRAMEWORK_UI_TERMINAL_TOOKIT_TOPLEVELCONTAINER

