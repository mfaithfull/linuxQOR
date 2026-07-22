// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/framework/app/role/role.h"

/*Include a custom Feature class. You can implement the features
of your application as custom QOR Features or just use the builtin ones
and add your own way to manage custom functionality*/
#include "customfeature.h"

/*Create a custom Role class by deriving from Role
Only derive directly from IRole if you plan to implement
your own feature storage and retreival mechanism to 
replace the built in QOR Role completely.*/

class CustomRole : public qor::app::Role
{
public:

    virtual void Setup();
    virtual void Shutdown();

};
