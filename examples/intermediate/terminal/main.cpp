// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "termapprole.h"
#include "src/framework/app/role/getfeature.h"

using namespace qor;
using namespace qor::log;
using namespace qor::ui;

qor_pp_implement_module("Terminal App Example")

int main()
{
    DefaultErrorHandler errorHandler;
    DefaultLogHandler logHandler(Level::Debug);    
    
    qor_pp_run_role(TermAppRole)(
        []()->int
        {
            auto terminal = GetFeature<Terminal>();
            return EXIT_SUCCESS;
        }
    );
}
