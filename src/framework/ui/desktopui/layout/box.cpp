// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <algorithm>

#include "box.h"

namespace qor{ namespace ui{ namespace layout {

    Box Box::Intersection(Box a, Box b) 
    {
        return Box
        {
            std::max(a.x_min, b.x_min),
            std::min(a.x_max, b.x_max),
            std::max(a.y_min, b.y_min),
            std::min(a.y_max, b.y_max),
        };
    }

    //return the smallest Box containing both |a| and |b|.
    Box Box::Union(Box a, Box b) 
    {
        return Box
        {
            std::min(a.x_min, b.x_min),
            std::max(a.x_max, b.x_max),
            std::min(a.y_min, b.y_min),
            std::max(a.y_max, b.y_max),
        };
    }

    // Shift the box by (x,y).
    void Box::Shift(int x, int y) 
    {
        x_min += x;
        x_max += x;
        y_min += y;
        y_max += y;
    }

    //return whether (x,y) is contained inside the box.
    bool Box::Contain(int x, int y) const 
    {
        return x_min <= x && x_max >= x && y_min <= y && y_max >= y;
    }

    //return whether the box is empty.
    bool Box::IsEmpty() const 
    {
        return x_min > x_max || y_min > y_max;
    }

    //return whether |other| is the same as |this|
    bool Box::operator==(const Box& other) const 
    {
        return (x_min == other.x_min) && (x_max == other.x_max) && (y_min == other.y_min) && (y_max == other.y_max);
    }

    //return whether |other| and |this| are different.
    bool Box::operator!=(const Box& other) const 
    {
        return !operator==(other);
    }

}}}//qor::ui::layout
