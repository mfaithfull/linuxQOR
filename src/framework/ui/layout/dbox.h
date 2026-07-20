// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_LAYOUT_DBOX
#define QOR_PP_H_FRAMEWORK_UI_LAYOUT_DBOX

#include "arranger.h"

namespace qor{ namespace ui{ namespace layout { 

    class DBox : public Arranger
    {
    public:
        
        DBox();
        virtual ~DBox();
    
        virtual void ComputeRequirement() override;
        virtual void SetBox(Box box) override;

    };

}}}//qor::ui:layout

#endif//QOR_PP_H_FRAMEWORK_UI_LAYOUT_DBOX
