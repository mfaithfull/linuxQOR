// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_FILESYSTEM_FILESTAT
#define QOR_PP_H_PLATFORM_FILESYSTEM_FILESTAT

#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "fileindex.h"

namespace qor{ namespace io{ namespace filesystem {

    class qor_pp_module_interface(QOR_FILESYSTEM) Stat
	{
	public:

        Stat();
        Stat(const Index& index);
        virtual ~Stat() noexcept;

        bool IsBlockFile();
        bool IsCharacterFile();
        bool IsEmpty();
        bool IsFIFO();
        bool IsOther();
        bool IsRegularFile();
        bool IsSocket();
        bool IsSymLink();
        bool IsValid();
        bool IsFile();
        bool IsDir();

    protected:

        Index m_index;        

    };

}}}//qor::io::filesystem

#endif//QOR_PP_H_PLATFORM_FILESYSTEM_FILESTAT
