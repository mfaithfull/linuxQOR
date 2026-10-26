// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_API_SHELL
#define QOR_PP_H_OS_WINDOWS_API_SHELL

#include "src/platform/os/windows/api_layer/kernel/kernel32.h"
//The C level prototypes for the shell32.dll entry points are declared by the Windows SDK in shellapi.h - that header is
//the source of truth for the signatures in this class. Verified against the exports of shell32.dll on the target system;
//see purpose.md for the audit. The SDK declares all of shellapi.h inside WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP).
#include <shellapi.h>


namespace qor { namespace win { namespace api {

	class qor_pp_module_interface(QOR_WINAPISHELL) Shell
	{
	public:

	//Shell execution and command line functions

		 static HINSTANCE ShellExecuteA( ::HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd );
		 static HINSTANCE ShellExecuteW( ::HWND hwnd, LPCWSTR lpOperation, LPCWSTR lpFile, LPCWSTR lpParameters, LPCWSTR lpDirectory, INT nShowCmd );
		 static BOOL ShellExecuteExA( ::SHELLEXECUTEINFOA *pExecInfo );
		 static BOOL ShellExecuteExW( ::SHELLEXECUTEINFOW *pExecInfo );
		 static INT ShellAboutA( ::HWND hWnd, LPCSTR szApp, LPCSTR szOtherStuff, ::HICON hIcon );
		 static INT ShellAboutW( ::HWND hWnd, LPCWSTR szApp, LPCWSTR szOtherStuff, ::HICON hIcon );
		 static HINSTANCE FindExecutableA( LPCSTR lpFile, LPCSTR lpDirectory, LPSTR lpResult );
		 static HINSTANCE FindExecutableW( LPCWSTR lpFile, LPCWSTR lpDirectory, LPWSTR lpResult );
		 static LPWSTR* CommandLineToArgvW( LPCWSTR lpCmdLine, INT* pNumArgs );
		 static DWORD DoEnvironmentSubstA( LPSTR pszSrc, UINT cchSrc );
		 static DWORD DoEnvironmentSubstW( LPWSTR pszSrc, UINT cchSrc );
		 static HRESULT SHEvaluateSystemCommandTemplate( PCWSTR pszCmdTemplate, PWSTR* ppszApplication, PWSTR* ppszCommandLine, PWSTR* ppszParameters );
		 static BOOL SHCreateProcessAsUserW( ::PSHCREATEPROCESSINFOW pscpi );
		 static BOOL SHTestTokenMembership( ::HANDLE hToken, ULONG ulRID );

		//Drag and drop functions

		 static void DragAcceptFiles( ::HWND hWnd, BOOL fAccept );
		 static void DragFinish( ::HDROP hDrop );
		 static UINT DragQueryFileA( ::HDROP hDrop, UINT iFile, LPSTR lpszFile, UINT cch );
		 static UINT DragQueryFileW( ::HDROP hDrop, UINT iFile, LPWSTR lpszFile, UINT cch );
		 static BOOL DragQueryPoint( ::HDROP hDrop, ::POINT* ppt );

		//Shell file operations

		 static int SHFileOperationA( ::LPSHFILEOPSTRUCTA* lpFileOp );
		 static int SHFileOperationW( ::LPSHFILEOPSTRUCTW* lpFileOp );
		 static void SHFreeNameMappings( ::HANDLE hNameMappings );
		 static BOOL SHGetDiskFreeSpaceExA( LPCSTR pszDirectoryName, ::ULARGE_INTEGER* pulFreeBytesAvailableToCaller, ::ULARGE_INTEGER* pulTotalNumberOfBytes, ::ULARGE_INTEGER* pulTotalNumberOfFreeBytes );
		 static BOOL SHGetDiskFreeSpaceExW( LPCWSTR pszDirectoryName, ::ULARGE_INTEGER* pulFreeBytesAvailableToCaller, ::ULARGE_INTEGER* pulTotalNumberOfBytes, ::ULARGE_INTEGER* pulTotalNumberOfFreeBytes );
		 static DWORD_PTR SHGetFileInfoA( LPCSTR pszPath, DWORD dwFileAttributes, ::SHFILEINFOA* psfi, UINT cbFileInfo, UINT uFlags );
		 static DWORD_PTR SHGetFileInfoW( LPCWSTR pszPath, DWORD dwFileAttributes, ::SHFILEINFOW* psfi, UINT cbFileInfo, UINT uFlags );
		 static BOOL SHGetNewLinkInfoA( LPCSTR pszLinkTo, LPCSTR pszDir, LPSTR pszName, BOOL* pfMustCopy, UINT uFlags );
		 static BOOL SHGetNewLinkInfoW( LPCWSTR pszLinkTo, LPCWSTR pszDir, LPWSTR pszName, BOOL* pfMustCopy, UINT uFlags );
		 static BOOL IsLFNDriveA( LPCSTR pszPath );
		 static BOOL IsLFNDriveW( LPCWSTR pszPath );

		//Icon and image list functions

		 static ::HICON DuplicateIcon( ::HINSTANCE hInst, ::HICON hIcon );
		 static ::HICON ExtractAssociatedIconA( ::HINSTANCE hInst, LPSTR pszIconPath, WORD* piIcon );
		 static ::HICON ExtractAssociatedIconW( ::HINSTANCE hInst, LPWSTR pszIconPath, WORD* piIcon );
		 static ::HICON ExtractAssociatedIconExA( ::HINSTANCE hInst, LPSTR pszIconPath, WORD* piIconIndex, WORD* piIconId );
		 static ::HICON ExtractAssociatedIconExW( ::HINSTANCE hInst, LPWSTR pszIconPath, WORD* piIconIndex, WORD* piIconId );
		 static ::HICON ExtractIconA( ::HINSTANCE hInst, LPCSTR pszExeFileName, UINT nIconIndex );
		 static ::HICON ExtractIconW( ::HINSTANCE hInst, LPCWSTR pszExeFileName, UINT nIconIndex );
		 static UINT ExtractIconExA( LPCSTR lpszFile, INT nIconIndex, ::HICON* phiconLarge, ::HICON* phiconSmall, UINT nIcons );
		 static UINT ExtractIconExW( LPCWSTR lpszFile, INT nIconIndex, ::HICON* phiconLarge, ::HICON* phiconSmall, UINT nIcons );
		 static HRESULT SHGetStockIconInfo( ::SHSTOCKICONID siid, UINT uFlags, ::SHSTOCKICONINFO* psii );
		 static HRESULT SHGetImageList( INT iImageList, REFIID riid, void** ppvObj );
		 static HRESULT SHLoadNonloadedIconOverlayIdentifiers( void );

		//Shell notification and status functions

		 static BOOL Shell_NotifyIconA( DWORD dwMessage, ::PNOTIFYICONDATAA* lpData );
		 static BOOL Shell_NotifyIconW( DWORD dwMessage, ::PNOTIFYICONDATAW* lpData );
		 static HRESULT Shell_NotifyIconGetRect( const ::NOTIFYICONIDENTIFIER* identifier, ::RECT* iconLocation );
		 static HRESULT SHQueryUserNotificationState( ::QUERY_USER_NOTIFICATION_STATE* pquns );
		 static HRESULT SHIsFileAvailableOffline( PCWSTR pwszPath, DWORD* pdwStatus );
		 static DWORD_PTR SHAppBarMessage( DWORD dwMessage, ::PAPPBARDATA* pData );
		 static BOOL InitNetworkAddressControl( void );

		//Recycle bin and printer command functions

		 static HRESULT SHEmptyRecycleBinA( ::HWND hwnd, LPCSTR pszRootPath, DWORD dwFlags );
		 static HRESULT SHEmptyRecycleBinW( ::HWND hwnd, LPCWSTR pszRootPath, DWORD dwFlags );
		 static HRESULT SHQueryRecycleBinA( LPCSTR pszRootPath, ::LPSHQUERYRBINFO* pSHQueryRBInfo );
		 static HRESULT SHQueryRecycleBinW( LPCWSTR pszRootPath, ::LPSHQUERYRBINFO* pSHQueryRBInfo );
		 static BOOL SHInvokePrinterCommandA( ::HWND hwnd, UINT uAction, LPCSTR lpBuf1, LPCSTR lpBuf2, BOOL fModal );
		 static BOOL SHInvokePrinterCommandW( ::HWND hwnd, UINT uAction, LPCWSTR lpBuf1, LPCWSTR lpBuf2, BOOL fModal );

		//Localized names and shell object property functions

		 static HRESULT SHSetLocalizedName( PCWSTR pszPath, PCWSTR pszResModule, INT idsRes );
		 static HRESULT SHRemoveLocalizedName( PCWSTR pszPath );
		 static HRESULT SHGetLocalizedName( PCWSTR pszPath, PWSTR pszResModule, UINT cch, INT* pidsRes );
		 static HRESULT AssocCreateForClasses( const ::ASSOCIATIONELEMENT* rgClasses, ULONG cClasses, REFIID riid, void** ppv );
		 static HRESULT SHGetPropertyStoreForWindow( ::HWND hwnd, REFIID riid, void** ppv );

    };

}}}//qor::win::api

#endif//QOR_PP_H_OS_WINDOWS_API_SHELL
