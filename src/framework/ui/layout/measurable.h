
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_MEASURABLE
#define QOR_PP_H_FRAMEWORK_UI_MEASURABLE

#include "box.h"
#include "../item.h"

namespace qor{ namespace ui{ namespace layout {
    
    //Has a size that can be used for layout
    class IMeasurable
    {
        IMeasurable() = default;
        virtual ~IMeasurable() = default;

        Box Measure()
        {
            return Box();
        }
        
    private:
        
    };

}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_UI_MEASURABLE
