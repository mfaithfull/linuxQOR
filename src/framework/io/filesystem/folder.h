// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_FILESYSTEM_FOLDER
#define QOR_PP_H_PLATFORM_FILESYSTEM_FOLDER

#include <functional>
#include "path.h"
#include "fileindex.h"
#include "permissions.h"

namespace qor{ namespace io{ namespace filesystem{

    class qor_pp_module_interface(QOR_FILESYSTEM) Folder
	{
	public:
        Folder(const Folder& src);
        Folder(const Path& path);
        Folder& operator = (const Folder&);
        virtual ~Folder();
        bool Exists();
        static Folder Create(Path& newFolder);
        bool Copy(Path& destParent);
        bool Move(Path& destParent);
        bool Rename(const std::string& name);
        bool Delete();
        Permissions GetPermissions();
        void SetPermissions(const Permissions& permissions, const PermissionOptions& options);
        void Enumerate(const std::function <bool (Index&)>& f ) const;
        void CreateSymLinkTo(Path& target);
        class Path Path();

    protected:
        class Path m_path;
    };

}}}//qor::io::filesystem

#endif//QOR_PP_H_PLATFORM_FILESYSTEM_FOLDER
