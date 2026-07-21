// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "pushbutton.h"

namespace qor{ namespace ui{ namespace widgets { namespace tty {

    PushButton::PushButton(const std::string& text) : widgets::PushButton(text) { }

    PushButton::~PushButton() = default;

}}}}//qor::ui::widgets::tty
