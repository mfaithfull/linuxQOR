// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_FILESYSTEM_FILE
#define QOR_PP_H_WINDOWS_FILESYSTEM_FILE

#include "src/framework/io/filesystem/pipe/ipipe.h"
#include "src/framework/io/filesystem/fileindex.h"
#include "src/framework/io/filesystem/path.h"
#include "src/platform/os/windows/common/structures.h"

//Declaration must match the one in src/system/filesystem/pipe/ipipe.h
namespace qor{ bool qor_pp_module_interface(QOR_WINDOWSFILESYSTEM) ImplementsPipe(); }

//All types on this interface must be portable
namespace qor{ namespace io { namespace win{    

    class qor_pp_module_interface(QOR_WINDOWSFILESYSTEM) Pipe : public io::Pipe
    {
    public:

        Pipe();
        Pipe(const Pipe& src);
        Pipe(const filesystem::Path& path, const std::string& fileName, int openFor, int withFlags) : Pipe(filesystem::Index(path,fileName),openFor,withFlags) {}
        Pipe(const filesystem::Index& direntry, int openFor, int withFlags);
        Pipe(const Descriptor& iod);
        virtual ~Pipe();

        virtual task<int> AsyncRead(const qor::io::async::Interface& ioContext, byte* buffer, size_t byteCount, off_t offset) override;
        virtual task<int> AsyncWrite(const qor::io::async::Interface& ioContext, byte* buffer, size_t byteCount, off_t offset) override;
        virtual int64_t Read(byte* buffer, size_t byteCount, int64_t offset = -1) override;
        virtual int64_t Write(byte* buffer, size_t byteCount, int64_t offset = -1) override;

    private:

        /*
		bool CancelIo();
		bool CancelIoEx(void* lpOverlapped );
		void* Create(const char* lpFileName, unsigned long dwDesiredAccess, unsigned long dwShareMode, void* lpSecurityAttributes, unsigned long dwCreationDisposition, unsigned long dwFlagsAndAttributes, void* hTemplateFile);
		void* Create(const wchar_t* lpFileName, unsigned long dwDesiredAccess, unsigned long dwShareMode, void* lpSecurityAttributes, unsigned long dwCreationDisposition, unsigned long dwFlagsAndAttributes, void* hTemplateFile);
		bool GetBandwidthReservation(unsigned long& periodMilliseconds, unsigned long& bytesPerPeriod, bool& discardable, unsigned long& transferSize, unsigned long& numOutstandingRequests);
		bool GetInformationByHandle(platform::win::ByHandleFileInformation* fileInformation );
		bool GetInformationByHandleEx(platform::win::FileInfoByHandleClass FileInformationClass, void* fileInformation, unsigned long bufferSize);
		unsigned long GetFinalPathNameByHandleT(platform::win::TCHAR* filePath, unsigned long cchFilePath, unsigned long flags);
		bool Lock(unsigned long offsetLow, unsigned long offsetHigh, unsigned long numberOfBytesToLockLow, unsigned long numberOfBytesToLockHigh);
		bool LockEx(unsigned long flags, unsigned long numberOfBytesToLockLow, unsigned long numberOfBytesToLockHigh, void* overlapped);
		int Open(const char* fileName, platform::win::OFStruct* reOpenBuff, unsigned int style);
		bool Read(byte* lpBuffer, unsigned long NumberOfBytesToRead, unsigned long& NumberOfBytesRead, void* overlapped);
		bool ReadEx(byte* lpBuffer, unsigned long NumberOfBytesToRead, void* overlapped, overlappedCompletionRoutine completionRoutine);
		bool ReadScatter(platform::win::FileSegmentElement* aSegmentArray, unsigned long numberOfBytesToRead, void* overlapped);
		bool SetEnd();
		bool SetBandwidthReservation(unsigned long periodMilliseconds, unsigned long bytesPerPeriod, bool discardable, unsigned long& transferSize, unsigned long& numOutstandingRequests);
		bool SetCompletionNotificationModes(unsigned char flags);
		bool SetInformationByHandle(platform::win::FileInfoByHandleClass fileInformationClass, void* fileInformation, unsigned long bufferSize);
		bool SetOverlappedRange(unsigned char* overlappedRangeStart, unsigned long length);
		unsigned long SetPointer(long distanceToMove, long& distanceToMoveHigh, unsigned long moveMethod);
		bool SetPointerEx(platform::win::LARGE_INTEGER distanceToMove, platform::win::LARGE_INTEGER* newFilePointer, unsigned long moveMethod);
		bool SetShortNameT(const platform::win::TCHAR* shortName);
		bool SetValidDataLength(long long validDataLength);
		bool Unlock(unsigned long fileOffsetLow, unsigned long fileOffsetHigh, unsigned long numberOfBytesToUnlockLow, unsigned long numberOfBytesToUnlockHigh);
		bool UnlockEx(unsigned long numberOfBytesToUnlockLow, unsigned long numberOfBytesToUnlockHigh, void* overlapped);
		bool Write(const byte* lpBuffer, unsigned long NumberOfBytesToWrite, unsigned long& NumberOfBytesWritten, void* overlapped);
        bool WriteEx(const byte* lpBuffer, unsigned long NumberOfBytesToWrite, void* overlapped, overlappedCompletionRoutine completionRoutine);
		bool WriteGather(platform::win::FileSegmentElement* aSegmentArray, unsigned long numberOfBytesToWrite, void* overlapped);
        */

    };
}}}//qor::io::win

#endif//QOR_PP_H_WINDOWS_FILESYSTEM_PIPE
