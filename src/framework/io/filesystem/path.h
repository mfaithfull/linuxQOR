// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_FILESYSTEM_PATH
#define QOR_PP_H_PLATFORM_FILESYSTEM_PATH

#include <string>
#include <filesystem>

namespace qor{ namespace io{
    class qor_pp_module_interface(QOR_FILESYSTEM) FileSystem;
}}//qor::io

namespace qor{ namespace io{ namespace filesystem{

    class qor_pp_module_interface(QOR_FILESYSTEM) Path
    {
        friend class qor::io::FileSystem;

    public:

        Path();
        Path(const Path& src);
        Path(const std::string& path);
        virtual ~Path();
        Path& operator = (const Path& src);
        Path& operator /= (const std::string& folder);
        Path operator / (const std::string& folder) const;
        Path Parent() const;
        std::string Separator() const;
        std::string SelfIndicator() const;
        std::string ParentIndicator() const;
        unsigned short MaxElementLength() const;
        std::string RootIndicator() const;
        std::string ToString() const;
        std::string ToString(std::string FileName) const;
        void Clear();
        Path RootName() const;
        Path RootDirectory() const;
        Path RelativePath() const;
        bool IsEmpty() const;
        bool HasRootPath() const;
        bool HasRootName() const;
        bool HasRootDirectory() const;
        bool HasRelativePath() const;
        bool HasParentPath() const;
        bool IsAbsolute() const;
        bool IsRelative() const;
        bool IsSamePath(const Path& Compare) const;
        operator std::filesystem::path();
        operator const std::filesystem::path() const;

    private:

        static std::string s_separator;
        static std::string s_selfIndicator;
        static std::string s_parentIndicator;
        static std::string s_rootIndicator;
        static unsigned short s_maxElementLength;

    protected:

        std::filesystem::path m_path;

    };
    
}}}//qor::io::filesystem

#endif//QOR_PP_H_PLATFORM_FILESYSTEM_PATH