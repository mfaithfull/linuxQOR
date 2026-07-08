// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_FLEXBOXHELPER
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_FLEXBOXHELPER

#include <vector>
#include "flexboxconfig.h"

namespace qor{ namespace ui{ namespace layout { namespace flexboxhelper {

    // A block is a rectangle in the flexbox.
    struct Block 
    {
        // Input:
        int min_size_x = 0;
        int min_size_y = 0;
        int flex_grow_x = 0;
        int flex_grow_y = 0;
        int flex_shrink_x = 0;
        int flex_shrink_y = 0;

        // Output:
        int line{};
        int line_position{};
        int x = 0;
        int y = 0;
        int dim_x = 0;
        int dim_y = 0;
        bool overflow = false;
    };

    // A line is a row of blocks.
    struct Line 
    {
        std::vector<Block*> blocks;
        int x = 0;
        int y = 0;
        int dim_x = 0;
        int dim_y = 0;
    };

    struct Global 
    {
        std::vector<Block> blocks;
        std::vector<Line> lines;
        FlexboxConfig config;
        int size_x;
        int size_y;
    };

    void Compute(Global& global);
        
}}}}//qor::ui::layout::flexboxhelper

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_FLEXBOXHELPER

