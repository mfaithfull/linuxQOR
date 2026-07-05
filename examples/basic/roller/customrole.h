// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/framework/app/role/role.h"

/*Create a custom Role class by deriving from Role
Only derive directly from IRole if you plan to implement
your own feature storage and retreival mechanism to 
replace the built in QOR Role completely.*/

class CustomRole : public qor::app::Role
{
public:
    CustomRole();
    virtual ~CustomRole() = default;

    virtual void Setup();
    virtual void Shutdown();

    /*It's also possible to override AddFeature but unlikely you'd ever need to.
    unless you really need to do something highly custom on every feature add.
    In almost all cases the Setup function on the individual Feature is a better
    customisation point*/

};
