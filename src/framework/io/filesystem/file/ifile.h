// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_FILESYSTEM_FILE_INTERFACE
#define QOR_PP_H_PLATFORM_FILESYSTEM_FILE_INTERFACE

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
    bool qor_pp_import ImplementsFile(void);
#else
    bool qor_pp_export ImplementsFile(void);//All libraries providing an implementation of File need to export this function so that the linker can find them
#endif
}

namespace qor{ namespace io{

    using FileTime = std::filesystem::file_time_type;
    using FileStatus = std::filesystem::file_status;

    namespace filesystem
    {
        class qor_pp_module_interface(QOR_FILESYSTEM) Index;

        enum class Type : std::underlying_type_t<std::filesystem::file_type> {
            None        = static_cast< std::underlying_type_t<std::filesystem::file_type> >(std::filesystem::file_type::none),
            NotFound    = static_cast< std::underlying_type_t<std::filesystem::file_type> >(std::filesystem::file_type::not_found),
            Regular     = static_cast< std::underlying_type_t<std::filesystem::file_type> >(std::filesystem::file_type::regular),
            Folder      = static_cast< std::underlying_type_t<std::filesystem::file_type> >(std::filesystem::file_type::directory),
            SymLink     = static_cast< std::underlying_type_t<std::filesystem::file_type> >(std::filesystem::file_type::symlink),
            Block       = static_cast< std::underlying_type_t<std::filesystem::file_type> >(std::filesystem::file_type::block),
            Character   = static_cast< std::underlying_type_t<std::filesystem::file_type> >(std::filesystem::file_type::character),
            FIFO        = static_cast< std::underlying_type_t<std::filesystem::file_type> >(std::filesystem::file_type::fifo),
            Socket      = static_cast< std::underlying_type_t<std::filesystem::file_type> >(std::filesystem::file_type::socket),
            Unknown     = static_cast< std::underlying_type_t<std::filesystem::file_type> >(std::filesystem::file_type::unknown)
        };

        enum Whence
        {
            Set,
            Current,
            End
        };

    }

}}//qor::io

#endif//QOR_PP_H_PLATFORM_FILESYSTEM_FILE_INTERFACE
