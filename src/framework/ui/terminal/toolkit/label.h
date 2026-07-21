// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_TERMINAL_TOOLKIT_LABEL
#define QOR_PP_H_FRAMEWORK_UI_TERMINAL_TOOLKIT_LABEL

#include "src/framework/ui/widgets/label.h"

namespace qor{ namespace ui { namespace widgets { namespace tty{
    
    class Label : public widgets::Label
    {
    public:
        
        Label(const std::string& text);
        virtual ~Label();

        virtual void ComputeRequirement() override;
        virtual void Render(Renderer* renderer) override;
    };

}}}}//qor::ui::widgets::tty

#endif//QOR_PP_H_FRAMEWORK_UI_WIDGETS_LABEL

