// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_FILESYSTEM_PIPE_INTERFACE
#define QOR_PP_H_PLATFORM_FILESYSTEM_PIPE_INTERFACE

#include <vector>
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/io/iodescriptor.h"
#include "src/framework/parallel/task/task.h"
#include "src/framework/io/async/interface.h"
#include "../permissions.h"

namespace qor{
#ifdef QOR_FILESYSTEM
    bool qor_pp_import ImplementsPipe(void);
#else
    bool qor_pp_export ImplementsPipe(void);//All libraries providing an implementation of File need to export this function so that the linker can find them
#endif
}

namespace qor{ namespace io{

    namespace filesystem
    {

    }

}}//qor::io

#endif//QOR_PP_H_PLATFORM_FILESYSTEM_PIPE_INTERFACE
