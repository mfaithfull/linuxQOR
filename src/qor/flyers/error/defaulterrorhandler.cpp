// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <iostream>
#include "defaulterrorhandler.h"

namespace qor
{ 
    bool DefaultErrorHandler::Handle(const qor::Error& error)
    {
        std::cerr << qor::SeverityNames[static_cast<int>(error.what().GetSeverity())] << ": " << error.what().Content() << std::endl;
        return false;   //Default error handler reports errors but does not resolve them
    }
}//qor
