// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_CURRENTPROCESS
#define QOR_PP_H_FRAMEWORK_CURRENTPROCESS

#include <optional>
#include <vector>

#include "icurrentprocess.h"

namespace qor
{
    //All libraries providing an implementation of ICurrentProcess also need to export this function so that the linker can find them
    //Here we import it creating a requirement for an exported implementaton elsewhere
    //Implementators should include icurrentprocess.h and not this header to avoid linker confusion
    bool qor_pp_import ImplementsICurrentProcess();
}

#endif//QOR_PP_H_FRAMEWORK_CURRENTPROCESS
