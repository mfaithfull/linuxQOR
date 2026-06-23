// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "app.h"

/*Redirects the Appliation singleton factory to use the App factory
This ensures all requests to get an Application instance will really get
the single App instance
This makes it safe for QOR code to use new_ref<Application>() to access
the App instance without needing to know about App iff
it is derived from Application*/
SingletonRedirector<Application, App> App::m_sRedirect;
