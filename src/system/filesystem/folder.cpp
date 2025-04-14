// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "src/configuration/configuration.h"

#include <filesystem>
#include "folder.h"

namespace qor{ namespace system{

    Folder::Folder(const Folder& src)
    {
        *this = src;
    }

    Folder::Folder(const class Path& path) : m_path(path) {}

    Folder& Folder::operator = (const Folder& src)
    {
        if(&src != this)
        {
            m_path = src.m_path;
        }   
        return *this;     
    }

    void Folder::Create(class Path& newFolder)
    {
        std::filesystem::create_directory(newFolder);
    }

    void Folder::Copy( class Path& destinationParent )
    {
        std::filesystem::copy_file(m_path.operator std::filesystem::__cxx11::path(), destinationParent);
    }

    void Folder::Delete()
    {
        std::filesystem::remove_all(m_path);
    }

    void Folder::Enumerate( const std::function <bool (FileIndex&)>& f )
    {
        for (auto const& dir_entry : std::filesystem::directory_iterator{m_path}) 
        {
            FileIndex item(dir_entry);
            if( !f(item) )
            {
                break;
            }
        }
    }

    void Folder::CreateSymLinkTo(class Path& target)
    {
        std::filesystem::create_symlink(target, m_path);
    }

    class Path Folder::Path()
    {
        return m_path;
    }


}}//qor::system
