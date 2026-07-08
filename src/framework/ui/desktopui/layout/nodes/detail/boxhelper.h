// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_LAYOUT_DETAIL_BOXHELPER
#define QOR_PP_H_FRAMEWORK_UI_LAYOUT_DETAIL_BOXHELPER

#include <vector>

namespace qor{ namespace ui{ namespace layout { namespace detail {

    namespace boxhelper
    {
        struct Element 
        {
            // Input:
            int min_size = 0;
            int flex_grow = 0;
            int flex_shrink = 0;

            // Output;
            int size = 0;
        };

        void Compute(std::vector<Element>* elements, int target_size);
    }

}}}}//qor::ui::layout::detail

#endif//QOR_PP_H_FRAMEWORK_UI_LAYOUT_DETAIL_BOXHELPER

