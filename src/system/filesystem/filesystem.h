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

#ifndef QOR_PP_H_SYSTEM_FILESYSTEM_FILESYSTEM
#define QOR_PP_H_SYSTEM_FILESYSTEM_FILESYSTEM

#include <filesystem>
#include "ifilesystem.h"
#include "root.h"
#include "file.h"
#include "src/system/system/isubsystem.h"

namespace qor
{
    bool qor_pp_import ImplementsIFileSystem();//All libraries providing an implementation of IFileSystem also need to export this function so that the linker can find them
}

namespace qor{ namespace system{

    class qor_pp_module_interface(QOR_FILESYSTEM) FileSystem : public ISubsystem
    {
    public:

        FileSystem();
        virtual ~FileSystem() noexcept = default;

        virtual void Setup();
        virtual void Shutdown();

        const Root& GetRoot();
        Path CurrentPath();
        void CurrentPath(Path& path);

        ref_of<File>::type CreateFile(const FileIndex& index, const int withFlags) const;
        bool DeleteFile(const FileIndex& index) const;
        ref_of<File>::type OpenFile();
        bool CopyFile();
        bool MoveFile();

        std::filesystem::space_info Space(const Path& path);
        Path TempFolder();
                
    private:

        ref_of<IFileSystem>::type m_pimpl;
        Root m_root;
    };
    
    }//qor::system
    
    constexpr GUID FileSystemGUID = {0x3eea49d1, 0x3bc7, 0x429b, {0xb8, 0x0a, 0x70, 0xe3, 0xcf, 0x87, 0x05, 0xe0}};
    qor_pp_declare_guid_of(system::FileSystem,FileSystemGUID);
}//qor

#endif//QOR_PP_H_SYSTEM_FILESYSTEM_FILESYSTEM