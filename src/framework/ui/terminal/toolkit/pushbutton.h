// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_TERMINAL_TOOLKIT_PUSHBUTTON
#define QOR_PP_H_FRAMEWORK_UI_TERMINAL_TOOLKIT_PUSHBUTTON

#include "src/framework/ui/widgets/pushbutton.h"

namespace qor{ namespace ui { namespace widgets { namespace tty {
    
    class qor_pp_module_interface(QOR_TERMINAL) PushButton : public widgets::PushButton
    {
    public:
        
        PushButton(const std::string& text);
        virtual ~PushButton();        
    };

}}}}//qor::ui::widgets::tty

#endif//QOR_PP_H_FRAMEWORK_UI_TERMINAL_TOOLKIT_PUSHBUTTON

