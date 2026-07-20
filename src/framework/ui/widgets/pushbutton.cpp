// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "pushbutton.h"
#include "../layout/align.h"
#include "label.h"

namespace qor{ namespace ui{ namespace widgets {

    PushButton::PushButton(const std::string& text)
    {
        Add(new_ref<layout::Center>(new_ref<Label>(text)));
    }

    PushButton::~PushButton() = default;

}}}//qor::ui::widgets
