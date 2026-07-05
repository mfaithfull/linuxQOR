// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_FILESYSTEM_ROOT
#define QOR_PP_H_PLATFORM_FILESYSTEM_ROOT

#include <string>

#include "path.h"
#include "folder.h"

namespace qor{ namespace io { namespace filesystem{

    class qor_pp_module_interface(QOR_FILESYSTEM) Root
    {
    public:

        Root();
        Root(const Root&);
        virtual ~Root();
        void Setup();
        Root& operator = (const Root& src);
        class Path operator / (const std::string folder) const;
        class Path Path() const;
        class Folder Folder() const;
        const std::string Indicator() const;

    private:

        class Path m_path;

    };
    
}}}//qor::io::filesystem

#endif//QOR_PP_H_PLATFORM_FILESYSTEM_ROOT
