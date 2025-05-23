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

#ifndef QOR_PP_H_SYSTEM_FILESYSTEM_INTERFACE
#define QOR_PP_H_SYSTEM_FILESYSTEM_INTERFACE

#include "src/qor/instance/singleton.h"
#include "src/qor/factory/factory.h"
#include "src/qor/factory/externalfactory.h"
#include "fileindex.h"

namespace qor{ namespace system{

    class qor_pp_module_interface(QOR_FILESYSTEM) IFileSystem
    {
    public:

        enum OpenFor
        {
            Exec = 1,
            ReadOnly = 2,
            ReadWrite = 4,
            Search = 8,
            WriteOnly = 16,
        };

        enum WithFlags
        {
            Append = 1 << 0,
            CloseExec = 1 << 1,
            CloseFork = 1 << 2,
            Create = 1 << 3,
            Directory = 1 << 4,
            DSync = 1 << 5,
            Exclusive = 1 << 6,
            NoCTTY = 1 << 7,
            NoFollow = 1 << 8,
            NonBlock = 1 << 9,
            RSync = 1 << 10,
            Sync = 1 << 11,
            Truncate = 1 << 12,
            TTYInit = 1 << 13,
            TempFile = 1 << 14,
        };

        enum ShareMode
        {
            SetUserIDOnExecution    =04000,
            SetGroupIDOnExecution   =02000,
            DirRestrictedDelFlag    =01000,
            Owner_ReadWiteExecute   = 0700,
            Owner_Read              = 0400,
            Owner_Write             = 0200,
            Owner_Execute           = 0100,
            Group_ReadWriteExecute  =  070,
            Group_Read              =  040,
            Group_Write             =  020,
            Group_Execute           =  010,
            Other_ReadWriteExecute  =   07,
            Other_Read              =   04,
            Other_Write             =   02,
            Other_Execute           =   01
        };

        IFileSystem() = default;
        virtual ~IFileSystem() noexcept = default;

        virtual void Setup() {}
        virtual void Shutdown() {}

        virtual std::string PathSeparator() const { return "/"; }
        virtual std::string SelfIndicator() const { return "."; }
        virtual std::string ParentIndicator() const { return ".."; }
        virtual std::string RootIndicator() const { return "/"; }
        virtual unsigned short MaxElementLength() const { return 256; }
        
        virtual ref_of<File>::type CreateFile(const FileIndex& index, const int withFlags) const 
        {
            ref_of<File>::type result;
            return result;

        }

        virtual ref_of<File>::type OpenFile(const FileIndex& index, const int openFor, const int withFlags) const
        {
            ref_of<File>::type result;
            return result;

        }        

        virtual bool DeleteFile(const FileIndex& index) const
        {
            return false;
        }
        
    };
    
    }//qor::system
    
    qor_pp_declare_instancer_of(system::IFileSystem, SingletonInstancer);
    qor_pp_declare_factory_of(system::IFileSystem, ExternalFactory);
    constexpr GUID IFileSystemGUID = {0x3474967c, 0x0be1, 0x417d, { 0xab, 0x71, 0xd0, 0x21, 0x10, 0x16, 0x0e, 0x9f}};
    qor_pp_declare_guid_of(system::IFileSystem,IFileSystemGUID);

}//qor

#endif//QOR_PP_H_SYSTEM_FILESYSTEM_INTERFACE
