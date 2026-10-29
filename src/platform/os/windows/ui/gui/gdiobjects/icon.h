// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_ICON
#define QOR_PP_H_WINDOWS_GUI_ICON

#include <string>

#include "src/platform/compiler/compiler.h"
#include "gdiobject.h"
#include "src/platform/os/windows/common/handles/handle.h"
#include "src/platform/os/windows/common/constants.h"
#include "src/platform/os/windows/common/structures.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    struct IconInfo
    {
        int             fIcon;
        unsigned long   xHotspot;
        unsigned long   yHotspot;
        void*           hbmMask;
        void*           hbmColor;
    };

    struct IconInfoEx
    {
        unsigned long   cbSize;
        int             fIcon;
        unsigned long   xHotspot;
        unsigned long   yHotspot;
        void*           hbmMask;
        void*           hbmColor;
        unsigned short  wResID;
        TCHAR           szModName[MaxPath];
        TCHAR           szResName[MaxPath];
    };

    class qor_pp_module_interface(QOR_WINGUI) Icon
    {
    public:

        explicit Icon(const PrimitiveHandle& h, bool own = true);
        Icon(const Icon& src);
        Icon(void* hInstance, int nWidth, int nHeight, byte cPlanes, byte cBitsPixel, const byte* lpbANDbits, const byte* lpbXORbits);
        Icon(byte* presbits, unsigned long dwResSize, bool fIcon, unsigned long dwVer);
        Icon(byte* pbIconBits, unsigned long cbIconBits, bool fIcon, unsigned long dwVer, int cxDesired, int cyDesired, unsigned int uFlags);
        Icon(IconInfo& iconinfo);
        Icon(void* hInstance, std::string lpIconName);
        Icon(const wchar_t* id);
        virtual ~Icon();
        const PrimitiveHandle& GetHandle() const;
        Icon Clone();
        bool Draw(void* hDC, int X, int Y);
        bool DrawEx(void* hdc, int xLeft, int yTop, void* hIcon, int cxWidth, int cyWidth, unsigned int istepIfAniCur, const Handle& hbrFlickerFreeDraw, unsigned int diFlags);
        bool GetInfo(IconInfo& iconinfo);
        bool GetInfoExT(IconInfoEx& iconinfoex);
        static int LookupIdFromDirectory(byte* presbits, bool fIcon);
        static int LookupIdFromDirectoryEx(byte* presbits, bool fIcon, int cxDesired, int cyDesired, unsigned int Flags);
        static unsigned int PrivateExtractT(const std::string& lpszFile, int nIconIndex, int cxIcon, int cyIcon, void** phicon, unsigned int* piconid, unsigned int nIcons, unsigned int flags);

    protected:

        PrimitiveHandle m_handle;

    private:
        bool m_owning;
    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_ICON
