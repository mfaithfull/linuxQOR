// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_LAYOUT_HBOX
#define QOR_PP_H_FRAMEWORK_UI_LAYOUT_HBOX

#include "arranger.h"

namespace qor{ namespace ui{ namespace layout { 

    class HBox : public Arranger
    {
    public:
        
        HBox();
        virtual ~HBox();
    
        virtual void ComputeRequirement() override;
        virtual void SetBox(Box box) override;

    };

}}}//qor::ui:layout

#endif//QOR_PP_H_FRAMEWORK_UI_LAYOUT_HBOX
