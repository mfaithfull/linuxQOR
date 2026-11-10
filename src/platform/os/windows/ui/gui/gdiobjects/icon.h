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

    //U26 (api_layer/user, closed 11 Nov 2026 as D45) - which ICONINFOEX this means is an include-order property.
    //winuser.h declares the SDK type twice: an A shape of 560 bytes in one arm and a W shape of 1,080 bytes in the
    //other, and the arm is picked by UNICODE/_UNICODE, which in this tree is defined only at kernel32.h:7-10 under
    //qor_pp_unicode (src/configuration/configuration.h:13 = 1). The layer publishes ::PICONINFOEX at
    //src/platform/os/windows/api_layer/user/user32.h:442 and qor_pp_useswinapiAW pastes GetIconInfoExW unconditionally,
    //so a TU that reaches <windows.h> before kernel32.h compiles the 560-byte A shape against a 1,080-byte write:
    //no compile error, no link error, 520 bytes written past the end of this struct at icon.cpp:94, where
    //GetInfoExT hands this mirror to the layer as reinterpret_cast<PICONINFOEX>(&iconinfoex). This mirror is always
    //the W shape because its TCHAR comes from common/structures.h:57-65 (qor_pp_unicode), not from _UNICODE, so the
    //fork is on the layer's parameter type rather than on this struct - which is why closing the published-signature
    //half of U26 does not close this instance: the published header has to say which ICONINFOEX it means. Recorded,
    //not fixed: the wrapper's signature is the SDK prototype of the export it binds (D18), so the fix belongs to
    //this published header - spell szModName/szResName wchar_t, or pin the shape with
    //static_assert(sizeof(IconInfoEx) == 1080) - and this is winqor_gui surface, hence the U20 rule.
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

        //U20 - the same ownership default, and the Icon half of the model disagrees with the Cursor half about LoadIcon.
        //(Note landed by the winqor_api_user review; its entry is src/platform/os/windows/api_layer/user/registers/u20.md,
        //opened 30 Oct 2026 (session 9, cursor.cpp) and closed 11 Nov 2026 by decision D47. The Icon half was measured
        //rather than inferred on 1 Nov 2026 (session 18, icon.cpp). The cites below are that entry's, carried as
        //written - nothing in this file was re-audited for them.)
        //A handle carries no ownership; see the ownership paragraph at cursor.h above. The release verb is per family,
        //and it is not Close for any of them: HICON releases as DestroyIcon (user32.h:438), HCURSOR as DestroyCursor
        //(:391), HWND as DestroyWindow (:152), HDC as ReleaseDC (:339) with the hWnd/hDC pair required to match, an
        //HENHMETAFILE as DeleteEnhMetaFile (gdi32.h:251) while CloseEnhMetaFile (gdi32.h:245) creates one.
        //The six acquiring ctors below split the two ownership states, and they split them wrongly. CreateIcon
        //(icon.cpp:24), CreateIconFromResource (:30), CreateIconFromResourceEx (:36) and CreateIconIndirect (:42)
        //return handles this process owns and must DestroyIcon. The two LoadIconT ctors - icon.cpp:46-50
        //(hInstance, name) and :52-56 (const wchar_t* id) - do not, yet both set m_owning = true, which is the Cursor
        //family's answer to the same call inverted: the identical Cursor ctors set it false (cursor.cpp:35-41, :49-53).
        //Measured against user32.dll, not inferred: LoadIconW(NULL, IDI_APPLICATION) -> 0x1002B and a second load of
        //the same id returns the same value, so it is one shared handle rather than a fresh one; DestroyIcon on it
        //returns 1 with GetLastError 1402 (ERROR_INVALID_HANDLE), a second DestroyIcon on the same value also returns
        //1, and GetIconInfo on it still succeeds afterwards - the predefined ids are not handle-table entries at all,
        //the value sits in the low word. The contract is violated and the OS absorbs it, which is why this has never
        //shown up as a failure.
        //Recorded, not fixed: the default is on this class's published interface, and moving it or moving m_owning in
        //the two LoadIconT ctors changes what every winqor_gui caller sees - a behaviour change in another library,
        //which the api_layer review records rather than performs (the rule U20 states, closed by D47).


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
