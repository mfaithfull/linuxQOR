// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "customrole.h"

void CustomRole::Setup()
{
    /*Code here runs before any features are Setup. 
    Warning: No services will be available*/

    /*Always call the base. 
    Usually call this first and then do extra customisation
    using the features already setup*/
    Role::Setup();

    /*Code here runs after all featues are Setup. This is the first point where all
    services will be available*/
}

void CustomRole::Shutdown()
{
    /*At this point all features are still available but should only be used to
    clean up*/

    /* Always call the base. 
    Almost always call this last.*/    
    Role::Shutdown();

    /*Code here runs after all features are shutdown. There is very little it is 
    safe to do here unless it only uses the Standard Library and statically linked
    QOR libraries*/
}