// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_WIDGETS_TOPLEVELCONTAINER
#define QOR_PP_H_FRAMEWORK_UI_WIDGETS_TOPLEVELCONTAINER

#include "widget.h"
#include "../common/activateable.h"

namespace qor{ namespace ui { namespace widgets {
    
    class TopLevelContainer : public Compound, 
        public virtual IActivatable, 
        public virtual IRenderable
    {
    public:
        
        TopLevelContainer() = default;
        virtual ~TopLevelContainer() = default;

    protected:

        layout::Box m_box;
    };

}}}//qor::ui::widgets

#endif//QOR_PP_H_FRAMEWORK_UI_WIDGETS_TOPLEVELCONTAINER

