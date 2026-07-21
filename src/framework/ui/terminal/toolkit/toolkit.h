// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_TERMINAL_TOOLKIT
#define QOR_PP_H_FRAMEWORK_UI_TERMINAL_TOOLKIT

#include "src/framework/ui/widgets/toolkit.h"
#include "tlc.h"

//A factory for the terminal specific derivatives of the widget set
namespace qor{ namespace ui{ namespace tty{

    class Toolkit : public ui::Toolkit
    {
    public:

        Toolkit() = default;
        ~Toolkit() = default;

        virtual ref_of<Item>::type CreateTopLevelContainer()
        {
            //Create a Top Level Terminal Window with it's own Compositor Surface. It's own Renderer instance?
            return new_ref<ui::widgets::tty::TopLevelContainer>();
        }
    };

}}}//qor::ui::tty

#endif//QOR_PP_H_FRAMEWORK_UI_TERMINAL_TOOLKIT
