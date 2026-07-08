// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "flexboxconfig.h"

namespace qor{ namespace ui{ namespace layout {

    //Set the flexbox direction.
    FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::Direction d) 
    {
        this->direction = d;
        return *this;
    }

    //Set the flexbox wrap.
    FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::Wrap w) 
    {
        this->wrap = w;
        return *this;
    }

    //Set the flexbox justify content.
    FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::JustifyContent j) 
    {
        this->justify_content = j;
        return *this;
    }

    //Set the flexbox align items.
    FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::AlignItems a) 
    {
        this->align_items = a;
        return *this;
    }

    //Set the flexbox align content.
    FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::AlignContent a) 
    {
        this->align_content = a;
        return *this;
    }

    //Set the flexbox flex direction.
    FlexboxConfig& FlexboxConfig::SetGap(int x, int y) 
    {
        this->gap_x = x;
        this->gap_y = y;
        return *this;
    }

}}}//qor::ui::layout
