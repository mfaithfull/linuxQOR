// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_LAYOUT_GRIDBOX
#define QOR_PP_H_FRAMEWORK_UI_LAYOUT_GRIDBOX

#include "arranger.h"

namespace qor{ namespace ui{ namespace layout { 

    class GridBox : public Arranger
    {
    public:
        
        GridBox();
        virtual ~GridBox();
    
        virtual void ComputeRequirement() override;
        virtual void SetBox(Box box) override;

    private:

        int x_size = 0;
        int y_size = 0;
    
    };

}}}//qor::ui:layout

#endif//QOR_PP_H_FRAMEWORK_UI_LAYOUT_GRIDBOX
