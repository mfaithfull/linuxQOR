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
            //There steps should end up as states in a Workflow which runs until the GUI exits
            
            auto terminal = GetFeature<Terminal>();
            auto dimensions = terminal->Size();
            auto toolkit = terminal->GetToolkit();
            auto renderer = terminal->GetRenderer();

            //Build a SceneTemplate

            //Create a scene from the SceneTemplate with the toolkit
            //auto scene = Scene::FromTemplate(toolkit);

            //Frame
            //renderer->Render(scene);
            //renderer->Present();

            return EXIT_SUCCESS;
        }
    );
}
