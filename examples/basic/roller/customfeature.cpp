// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <iostream>
#include "customfeature.h"

void CustomFeature::ConfigureMessage(const std::string& message)
{
    m_message = message;
}

void CustomFeature::SayHello()
{
    std::cout << m_message << std::endl;
}

