// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "gdi32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //MetaFile and EnhMetaFile functions - see gdi/purpose.md for the binding convention.
    //Names that wingdi.h #defines (CopyEnhMetaFile, CreateMetaFile, GetEnhMetaFile ...) are passed to
    //qor_pp_useswinapiAW unsuffixed: the ## token paste in the macro suppresses that expansion and yields
    //the W suffix export (CopyEnhMetaFileW, CreateMetaFileW, GetEnhMetaFileW ...), all present in
    //tmp/_tmp_gdi_exports.txt.

    HENHMETAFILE GDI32::CloseEnhMetaFile(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CloseEnhMetaFile);
        return Library::Call< HENHMETAFILE, HDC >(pFunc, hdc);
    }

    HMETAFILE GDI32::CloseMetaFile(HDC hdc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, CloseMetaFile);
        return Library::Call< HMETAFILE, HDC >(pFunc, hdc);
    }

    HENHMETAFILE GDI32::CopyEnhMetaFile(HENHMETAFILE hemfSrc, LPCTSTR lpszFile)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, CopyEnhMetaFile);
        return Library::Call< HENHMETAFILE, HENHMETAFILE, LPCTSTR >(pFunc, hemfSrc, lpszFile);
    }

    HMETAFILE GDI32::CopyMetaFile(HMETAFILE hmfSrc, LPCTSTR lpszFile)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, CopyMetaFile);
        return Library::Call< HMETAFILE, HMETAFILE, LPCTSTR >(pFunc, hmfSrc, lpszFile);
    }

    HDC GDI32::CreateEnhMetaFile(HDC hdcRef, LPCTSTR lpFilename, CONST::RECT* lpRect, LPCTSTR lpDescription)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, CreateEnhMetaFile);
        return Library::Call< HDC, HDC, LPCTSTR, CONST ::RECT*, LPCTSTR >(pFunc, hdcRef, lpFilename, lpRect, lpDescription);
    }

    HDC GDI32::CreateMetaFile(LPCTSTR lpszFile)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, CreateMetaFile);
        return Library::Call< HDC, LPCTSTR >(pFunc, lpszFile);
    }

    BOOL GDI32::DeleteEnhMetaFile(HENHMETAFILE hemf)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, DeleteEnhMetaFile);
        return Library::Call< BOOL, HENHMETAFILE >(pFunc, hemf);
    }

    BOOL GDI32::DeleteMetaFile(HMETAFILE hmf)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, DeleteMetaFile);
        return Library::Call< BOOL, HMETAFILE >(pFunc, hmf);
    }

    BOOL GDI32::EnumEnhMetaFile(HDC hdc, HENHMETAFILE hemf, ::ENHMFENUMPROC lpEnhMetaFunc, LPVOID lpData, CONST::RECT* lpRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, EnumEnhMetaFile);
        return Library::Call< BOOL, HDC, HENHMETAFILE, ::ENHMFENUMPROC, LPVOID, CONST ::RECT* >(pFunc, hdc, hemf, lpEnhMetaFunc, lpData, lpRect);
    }

    BOOL GDI32::EnumMetaFile(HDC hdc, HMETAFILE hmf, ::MFENUMPROC lpMetaFunc, LPARAM lParam)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, EnumMetaFile);
        return Library::Call< BOOL, HDC, HMETAFILE, ::MFENUMPROC, LPARAM >(pFunc, hdc, hmf, lpMetaFunc, lParam);
    }

    BOOL GDI32::GdiComment(HDC hdc, UINT cbSize, CONST BYTE* lpData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GdiComment);
        return Library::Call< BOOL, HDC, UINT, CONST BYTE* >(pFunc, hdc, cbSize, lpData);
    }

    HENHMETAFILE GDI32::GetEnhMetaFile(LPCTSTR lpszMetaFile)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, GetEnhMetaFile);
        return Library::Call< HENHMETAFILE, LPCTSTR >(pFunc, lpszMetaFile);
    }

    UINT GDI32::GetEnhMetaFileBits(HENHMETAFILE hemf, UINT cbBuffer, LPBYTE lpbBuffer)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetEnhMetaFileBits);
        return Library::Call< UINT, HENHMETAFILE, UINT, LPBYTE >(pFunc, hemf, cbBuffer, lpbBuffer);
    }

    UINT GDI32::GetEnhMetaFileDescription(HENHMETAFILE hemf, UINT cchBuffer, LPTSTR lpszDescription)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, GetEnhMetaFileDescription);
        return Library::Call< UINT, HENHMETAFILE, UINT, LPTSTR >(pFunc, hemf, cchBuffer, lpszDescription);
    }

    UINT GDI32::GetEnhMetaFileHeader(HENHMETAFILE hemf, UINT cbBuffer, ::LPENHMETAHEADER lpemh)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetEnhMetaFileHeader);
        return Library::Call< UINT, HENHMETAFILE, UINT, ::LPENHMETAHEADER >(pFunc, hemf, cbBuffer, lpemh);
    }

    UINT GDI32::GetEnhMetaFilePaletteEntries(HENHMETAFILE hemf, UINT cEntries, ::LPPALETTEENTRY lppe)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetEnhMetaFilePaletteEntries);
        return Library::Call< UINT, HENHMETAFILE, UINT, ::LPPALETTEENTRY >(pFunc, hemf, cEntries, lppe);
    }

    UINT GDI32::GetMetaFileBitsEx(HMETAFILE hmf, UINT nSize, LPVOID lpvData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetMetaFileBitsEx);
        return Library::Call< UINT, HMETAFILE, UINT, LPVOID >(pFunc, hmf, nSize, lpvData);
    }

    UINT GDI32::GetWinMetaFileBits(HENHMETAFILE hemf, UINT cbBuffer, LPBYTE lpbBuffer, INT fnMapMode, HDC hdcRef)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, GetWinMetaFileBits);
        return Library::Call< UINT, HENHMETAFILE, UINT, LPBYTE, INT, HDC >(pFunc, hemf, cbBuffer, lpbBuffer, fnMapMode, hdcRef);
    }

    HMETAFILE GDI32::GetMetaFile(LPCTSTR lpName)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(gdi32, GetMetaFile);
        return Library::Call< HMETAFILE, LPCTSTR >(pFunc, lpName);
    }

    BOOL GDI32::PlayEnhMetaFile(HDC hdc, HENHMETAFILE hemf, CONST::RECT* lpRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PlayEnhMetaFile);
        return Library::Call< BOOL, HDC, HENHMETAFILE, CONST ::RECT* >(pFunc, hdc, hemf, lpRect);
    }

    BOOL GDI32::PlayEnhMetaFileRecord(HDC hdc, LPHANDLETABLE lpHandletable, CONST ENHMETARECORD* lpEnhMetaRecord, UINT nHandles)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PlayEnhMetaFileRecord);
        return Library::Call< BOOL, HDC, LPHANDLETABLE, CONST ENHMETARECORD*, UINT >(pFunc, hdc, lpHandletable, lpEnhMetaRecord, nHandles);
    }

    BOOL GDI32::PlayMetaFile(HDC hdc, HMETAFILE hmf)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PlayMetaFile);
        return Library::Call< BOOL, HDC, HMETAFILE >(pFunc, hdc, hmf);
    }

    BOOL GDI32::PlayMetaFileRecord(HDC hdc, LPHANDLETABLE lpHandletable, LPMETARECORD lpMetaRecord, UINT nHandles)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, PlayMetaFileRecord);
        return Library::Call< BOOL, HDC, LPHANDLETABLE, LPMETARECORD, UINT >(pFunc, hdc, lpHandletable, lpMetaRecord, nHandles);
    }

    HENHMETAFILE GDI32::SetEnhMetaFileBits(UINT cbBuffer, CONST BYTE* lpData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetEnhMetaFileBits);
        return Library::Call< HENHMETAFILE, UINT, CONST BYTE* >(pFunc, cbBuffer, lpData);
    }

    HMETAFILE GDI32::SetMetaFileBitsEx(UINT nSize, CONST BYTE* lpData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetMetaFileBitsEx);
        return Library::Call< HMETAFILE, UINT, CONST BYTE* >(pFunc, nSize, lpData);
    }

    HENHMETAFILE GDI32::SetWinMetaFileBits(UINT cbBuffer, CONST BYTE* lpbBuffer, HDC hdcRef, CONST::METAFILEPICT* lpmfp)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(gdi32, SetWinMetaFileBits);
        return Library::Call< HENHMETAFILE, UINT, CONST BYTE*, HDC, CONST ::METAFILEPICT* >(pFunc, cbBuffer, lpbBuffer, hdcRef, lpmfp);
    }

}}}//qor::win::api
