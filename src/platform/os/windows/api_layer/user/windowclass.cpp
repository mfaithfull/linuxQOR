// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //=== api_layer conventions ========================================= Reviewed 4 Nov 2026
    //   (session 34: correctness / consistency / safety / UB / conventions; 14 wrappers)
    //
    //Fourteen wrappers - the whole "user32.h:619 //Window-Class functions" section: declared :620-:633, all static, all
    //defined here through the User32:: qualifier and in header order (:60 GetClassInfoT, :67, :74, :81, :88, :95, :102,
    //:109, :116, :123, :130, :137, :144, :151 UnregisterClassT), so 14/14 in-section declarations pair with a
    //definition: 0 U4 (declared, never defined), 0 U11 (definition unqualified), 0 U13 (defined, declared non-static),
    //0 U17 (the 12 members over an A/W family all carry the folder's T suffix, and GetClassWord/SetClassWord, whose
    //stems have no A/W family, correctly do not).
    //
    //Return types are the SDK's: tmp/_tmp_s34_names.ps1 compares declaration, definition and SDK prototype
    //case-sensitively over all 14 pairs and reports 0 return-type divergences (U18 clean). The LONG vs LONG_PTR pair the
    //pre-flags flagged is faithful - "GetWindowLongW" LONG at winuser.h:9868 against "Get/SetWindowLongPtrW" LONG_PTR at
    //:9908, :9927 and "Get/SetClassLongPtrW" ULONG_PTR at :10031, :10050 - so no wrapper truncates. Parameter names
    //match the header on 14/14 and the SDK on 12/14 (hinst at :67 against the SDK's hInstance, lpwcx at :109), published
    //names only; the 6 TYPE rows are all the layer's TCHAR-to-W mapping (LPCTSTR against LPCWSTR, LPWNDCLASS against
    //LPWNDCLASSW), which is U26's convention rather than a defect.
    //
    //Binding (D13): 12 qor_pp_useswinapiAW (:63, :70, :77, :84, :98, :105, :112, :119, :133, :140, :147, :154) and 2
    //qor_pp_useswinapi (:91, :126). Each of the 12 AW stems has "<stem>A" and "<stem>W" exports and NO bare export in
    //tmp/_tmp_user32_exports.txt, and GetClassWord/SetClassWord are bare singles with no A/W variant, so both spellings
    //are the only ones that could resolve at all. cl /EP over this TU (tmp/_tmp_win_pp_s34.bat read by
    //tmp/_tmp_win_ppclass_s33.ps1 -> tmp/_tmp_s34_ppclass.txt): 14 binding lines, 14 distinct literals, 0 absent from
    //the export table, 0 ...WW, 0 ...T, module literal L"user32" 14/14.
    //
    //ARCH DEFECT - RECORDED, NOT FIXED (registers/u28.md): the four *Ptr members bind Get/Set*LongPtrW at :77, :119,
    //:140 and :147, and those four names exist only in the 64-bit image: winuser.h:9897 and :10020 put the *Ptr
    //prototypes under "#ifdef _WIN64", and the "#else" arms (:9939-:9940, :9947-:9948 for the window pair;
    //:10062-:10063, :10070-:10071 for the class pair) make *PtrA/*PtrW MACROS over the non-Ptr entry points, so 32-bit
    //user32.dll exports none of them. Live probe on both arches
    //(tmp/_tmp_s34_ptrprobe.cpp, built and run x64+x86 by tmp/_tmp_s34_ptrprobe.bat): x64 16/16 names RESOLVED; x86
    //GetWindowLongPtrW, SetWindowLongPtrW, GetClassLongPtrW and SetClassLongPtrW MISSING while every other name here
    //RESOLVED. pFunc is a function-local static const, so on x86 those four would cache NULL (G5) and throw "Missing
    //library function." on every call (U8). Latent, not live: x86 does not build at all (session 31 - 93 of 106
    //api_layer TUs fail at kernel32.h:92, U6), so the fix travels with that blocker rather than with this TU.
    //CLOSED 5 Nov 2026 by user decision D17 (docs/agent/DECISIONS.md): Windows is 64-bit (amd64) only, no 32-bit x86
    //(src/platform/os/windows/purpose.md:3), so that blocker is closed as not-to-be-fixed and these four bindings are the
    //correct spelling for the only supported image - no arch-conditional binding is to be added here.
    //
    //U16: only "RegisterClassExW" (winuser.h:4368) sits under a version floor - "#if(WINVER >= 0x0400)" at :4359. The other
    //13 sit under _WINUSER_/NOUSER plus WINAPI_FAMILY_PARTITION - DESKTOP alone at :4329 (GetClassInfoW) and :9963 (the
    //offsets block), DESKTOP | GAMES at :4289, :4357, :9855, :10209 - plus NOWINOFFSETS (:9965) and _WIN64 (:9897, :10020)
    //on the four *Ptr (tmp/_tmp_s34_u16.txt, depth-counted).
    //U25: this TU spells CONST (4 sites: :102, :106, :109, :113) like its two header declarations (user32.h:626, :627)
    //and uses no lowercase const - recorded, not restyled. T7: the #include "../returncheck.h" at (then) :11 was unused
    //here (0 CheckReturn/BoolCheck/Win32ErrorHandler) - dead, removed under T7 on 5 Nov 2026.

    BOOL User32::GetClassInfoT(HINSTANCE hInstance, LPCTSTR lpClassName, LPWNDCLASS lpWndClass)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetClassInfo);
        return Library::Call< BOOL, HINSTANCE, LPCTSTR, LPWNDCLASS >(pFunc, hInstance, lpClassName, lpWndClass);
    }

    BOOL User32::GetClassInfoExT(HINSTANCE hInstance, LPCTSTR lpszClass, LPWNDCLASSEX lpwcx)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetClassInfoEx);
        return Library::Call< BOOL, HINSTANCE, LPCTSTR, LPWNDCLASSEX >(pFunc, hInstance, lpszClass, lpwcx);
    }

    ULONG_PTR User32::GetClassLongPtrT(HWND hWnd, int nIndex)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetClassLongPtr);
        return Library::Call< ULONG_PTR, HWND, int >(pFunc, hWnd, nIndex);
    }

    int User32::GetClassNameT(HWND hWnd, LPTSTR lpClassName, int nMaxCount)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetClassName);
        return Library::Call< int, HWND, LPTSTR, int >(pFunc, hWnd, lpClassName, nMaxCount);
    }

    WORD User32::GetClassWord(HWND hWnd, int nIndex)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetClassWord);
        return Library::Call< WORD, HWND, int >(pFunc, hWnd, nIndex);
    }

    LONG User32::GetWindowLongT(HWND hWnd, int nIndex)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetWindowLong);
        return Library::Call< LONG, HWND, int >(pFunc, hWnd, nIndex);
    }

    ATOM User32::RegisterClassT(CONST WNDCLASS* lpWndClass)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, RegisterClass);
        return Library::Call< ATOM, CONST WNDCLASS* >(pFunc, lpWndClass);
    }

    ATOM User32::RegisterClassExT(CONST WNDCLASSEX* lpwcx)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, RegisterClassEx);
        return Library::Call< ATOM, CONST WNDCLASSEX* >(pFunc, lpwcx);
    }

    ULONG_PTR User32::SetClassLongPtrT(HWND hWnd, int nIndex, LONG_PTR dwNewLong)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetClassLongPtr);
        return Library::Call< ULONG_PTR, HWND, int, LONG_PTR >(pFunc, hWnd, nIndex, dwNewLong);
    }

    WORD User32::SetClassWord(HWND hWnd, int nIndex, WORD wNewWord)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetClassWord);
        return Library::Call< WORD, HWND, int, WORD >(pFunc, hWnd, nIndex, wNewWord);
    }

    LONG User32::SetWindowLongT(HWND hWnd, int nIndex, LONG dwNewLong)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetWindowLong);
        return Library::Call< LONG, HWND, int, LONG >(pFunc, hWnd, nIndex, dwNewLong);
    }

    LONG_PTR User32::SetWindowLongPtrT(HWND hWnd, int nIndex, LONG_PTR dwNewLong)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetWindowLongPtr);
        return Library::Call< LONG_PTR, HWND, int, LONG_PTR >(pFunc, hWnd, nIndex, dwNewLong);
    }

    LONG_PTR User32::GetWindowLongPtrT(HWND hWnd, int nIndex)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetWindowLongPtr);
        return Library::Call< LONG_PTR, HWND, int >(pFunc, hWnd, nIndex);
    }

    BOOL User32::UnregisterClassT(LPCTSTR lpClassName, HINSTANCE hInstance)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, UnregisterClass);
        return Library::Call< BOOL, LPCTSTR, HINSTANCE >(pFunc, lpClassName, hInstance);
    }

}}}//qor::win::api
