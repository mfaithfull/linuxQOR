// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "dbox.h"
#include "../functional/flexbox.h"
#include "detail/anyargs.h"

namespace qor{ namespace ui{ namespace layout {

    //Stack several element on top of each other.
    //children_ The input element.
    Element dbox(Elements children_) 
    {
        return new_ref<DBox>(std::move(children_));
    }

}}}//qor::ui::layout
