// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //=== api_layer conventions ========================================= Reviewed 4 Nov 2026
    //   (session 32: correctness / consistency / safety / UB / conventions; 4 wrappers)
    //
    //Four wrappers - the whole "user32.h:567 //Font and Text functions" section: declared :568-:571, all static, all
    //defined here through the User32:: qualifier and in header order (:58 DrawTextT, :65 DrawTextExT,
    //:72 GetTabbedTextExtentT, :79 TabbedTextOutT). 4/4 declarations pair with a definition here, so this TU
    //contributes 0 U4 (declared, never defined), 0 U11 (definition unqualified), 0 U13 (defined, declared
    //non-static), 0 U17 (A/W family without the folder's T suffix) and 0 U18 (return type contradicts the SDK):
    //tmp/_tmp_sigcmp.ps1 -Tu text.cpp -First 568 -Last 571 reports 4/4 declaration==definition for return type,
    //parameter types, names and Call<> arity, and its one "CALL TYPE MISMATCH" is the instrument's known
    //CONST::LPINT vs CONST ::LPINT artifact at :83, not a defect (gotchas.md). tmp/_tmp_s32_names.ps1: 0 header-vs-
    //definition parameter-name divergences; def-vs-SDK names diverge on 4/4 members (13 names, e.g. nCount/chCount, X/x),
    //published names only, and 0 def-vs-SDK type divergences over the four signatures.
    //
    //Binding (D13): all four stems are A/W-only - a word-boundary walk of tmp/_tmp_user32_exports.txt gives bare
    //DrawText, DrawTextEx, GetTabbedTextExtent and TabbedTextOut at count 0, while DrawTextA (DB)/DrawTextW (DE),
    //DrawTextExA (DC)/DrawTextExW (DD), GetTabbedTextExtentA (1CB)/W (1CC) and TabbedTextOutA (3AB)/W (3AC) are all
    //exported - so the qor_pp_useswinapiAW at :61, :68, :75 and :82 is the only spelling that can bind them, and P1,
    //P2 and P6 cannot arise. This is the folder's one TU where every bound stem has its SDK A/W macro live at the
    //binding site (#define DrawText DrawTextW at winuser.h:8269, DrawTextEx :8331, GetTabbedTextExtent :8494,
    //TabbedTextOut :8470, each with the A form under #else), which is what makes the spelling worth measuring: the
    //macro pastes W (_NAME##W, kernel32.h:52) before _NAME could be macro-expanded, so it cannot double-append.
    //cl /EP over this TU (tmp/_tmp_text_pp_s32.txt from :453254) emits exactly "DrawTextW", "DrawTextExW",
    //"GetTabbedTextExtentW" and "TabbedTextOutW" - 1 literal each, 0 bare, 0 double-suffixed, module literal
    //L"user32" on all four.
    //
    //Version floors (U16): all four SDK pairs are in the WINAPI_PARTITION_DESKTOP block; the DrawText pair sits
    //inside #ifndef NODRAWTEXT (closed winuser.h:8340) and the DrawTextEx pair additionally inside
    //#if(WINVER >= 0x0400) (opened :8301, closed :8335), so DrawTextExT is the only member here with a version floor;
    //TabbedTextOutW (:8460) and GetTabbedTextExtentW (:8487) have neither. NODRAWTEXT is a guard name this review has
    //not met before: defining it removes the two prototypes this TU wraps.
    //
    //Return policy (U1/U2): 0 Win32ErrorHandler and 0 CheckReturn here, and no check is sound across the four - the
    //SDK marks DrawTextA/W and DrawTextExA/W _Success_(return) (winuser.h:8245, :8257, :8303, :8317), so for those two
    //a non-zero test is what the SDK itself asserts, while TabbedTextOutA/W and GetTabbedTextExtentA/W carry no
    //annotation and return measurements (a width, and a packed LOWORD width/HIWORD height), where 0 is a legitimate
    //result. Same answer-shaped-value caution as rectangle.cpp's BOOL members (session 28).
    //
    //Two contracts this layer cannot state (recorded as register items, not changed here): the tab-stop array
    //parameter is CONST LPINT / CONST::LPINT at :72, :76, :79 and :83, which the probe tmp/_tmp_s32_u23_probe.cpp
    //proves is INT* const rather than the SDK's CONST INT * (U23, second instance after MessageBoxIndirectT); and
    //with the DT_MODIFYSTRING format flag the SDK treats the text buffer as _Inout_ and wants room for four more
    //characters (winuser.h:8250, :8309), which the layer's LPCTSTR cannot express - it matches the SDK's own LPCWSTR
    //spelling, so there is no divergence to fix, only a caller duty the signature hides.
    //closure: 4 declarations in range (user32.h:568-:571) -> 4 definitions, 0 static-scope globals, 0 functions not
    //   declared in the header.
    //P1 binding: all 4 literals resolve to real user32.dll exports
    //  - W-suffixed literal in a macro: 0/4 -> P2 0 instances (the macro pastes W before the name could expand, so it
    //    cannot double-append; /EP shows W exactly once on all four, and 4/4 would mis-bind to the non-existent
    //    DrawTextWW / DrawTextExWW / GetTabbedTextExtentWW / TabbedTextOutWW);
    //  - bare name bound for an A/W export: 0/4 - all four are bare at the call site and macro-expanded at the
    //    binding site, so the binding is correct BY CONSTRUCTION (the G1 case).
    //return-shape: 0 of 4 checked. BOOL-like returns 2/4 - DrawTextT and DrawTextExT are the SDK's two
    //   _Success_(return) members here, so for those a non-zero test is what the SDK itself asserts; the other two
    //   return measurements where 0 is legitimate -> U1/U2 folder-wide, nothing per-member.
    //U16 version gate: 4/4 members in WINAPI_PARTITION_DESKTOP; 1/4 has a version floor (DrawTextExT sits inside
    //   #if(WINVER >= 0x0400), opened winuser.h:8301, closed :8335 - DrawTextT is only NODRAWTEXT-gated, :8241/:8340;
    //   the TabbedTextOut/GetTabbedTextExtent pair at :8456-:8495 has neither floor).
    //U16 guard name: NODRAWTEXT (winuser.h:8241) - first time this review has met it; defining it removes the two
    //   DrawText prototypes.
    //U18: no instance in this TU (see the paragraph above the closure).
    //U25 (const placement): 8 sites (TabbedTextOutT is 2), all T-prefixed -> 0 `const` keyword, 0 U25.
    //U23 (contract the layer cannot state): 2 instances here - (1) CONST LPINT binds INT* const, not the SDK's
    //   CONST INT * (the whole tab-stop array is writable through this wrapper), probe tmp/_tmp_s32_u23_probe.cpp;
    //   (2) DT_MODIFYSTRING makes the SDK treat the text buffer as _Inout_ with room for four more characters
    //   (winuser.h:8250, :8309) while the wrapper says LPCTSTR (it matches the SDK's own LPCWSTR spelling, so there
    //   is no divergence to fix - the caller duty is what the signature hides).
    //P5 call-paren: 4 tight / 0 spaced (Library::Call<...) on this TU's 4 call sites.
    //U19: 1/4 LAYER-reachable - the DT_* format flags (19 of them) appear only inside comments in-tree (0 flag uses),
    //   the sole caller passes DT_EDITCONTROL (:229, and it is not in the SDK's DT_* macro list); U19 0 instances.
    //U15: D14 - 0 instances. U4 0, U11 0, U13 0, U17 0. U26: 0 SDK structs mirrored (no tag types in the four
    //   signatures; RECT/LPRECT are referenced, not re-declared), so U26 stays at 4.
    //caller-side: 0 callers in api_layer/user for all four stems; all four have in-tree consumers outside the folder
    //   (12 in ui/gui/painting/themedata.cpp over 13 call parens, DT_EDITCONTROL at :229; 2 in winqor_kernelobj.cpp),
    //   so the family is live surface -> 0 wrong-stem rows, 0 dead exports (P5 stays 428).
    //The next TU is `window.cpp` - the folder's largest (**measured 85 wrappers over 615 lines; the census row says 50**):
    //   it carries the U26 `TagRECT` mirror question, U19's `TrackPopupMenu` BYPASS split and U27's `SetScrollInfo`
    //   question, then `windowclass.cpp`, `windowprocedure.cpp`, `windowproperties.cpp` and the `winqor_api_user.cpp`
    //   glue TU.


    int User32::DrawTextT(HDC hDC, LPCTSTR lpString, int nCount, ::LPRECT lpRect, UINT uFormat)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DrawText);
        return Library::Call< int, HDC, LPCTSTR, int, ::LPRECT, UINT >(pFunc, hDC, lpString, nCount, lpRect, uFormat);
    }

    int User32::DrawTextExT(HDC hdc, LPTSTR lpchText, int cchText, ::LPRECT lprc, UINT dwDTFormat, ::LPDRAWTEXTPARAMS lpDTParams)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, DrawTextEx);
        return Library::Call< int, HDC, LPTSTR, int, ::LPRECT, UINT, ::LPDRAWTEXTPARAMS >(pFunc, hdc, lpchText, cchText, lprc, dwDTFormat, lpDTParams);
    }

    DWORD User32::GetTabbedTextExtentT(HDC hDC, LPCTSTR lpString, int nCount, int nTabPositions, CONST LPINT lpnTabStopPositions)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetTabbedTextExtent);
        return Library::Call< DWORD, HDC, LPCTSTR, int, int, CONST LPINT >(pFunc, hDC, lpString, nCount, nTabPositions, lpnTabStopPositions);
    }

    LONG User32::TabbedTextOutT(HDC hDC, int X, int Y, LPCTSTR lpString, int nCount, int nTabPositions, CONST::LPINT lpnTabStopPositions, int nTabOrigin)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, TabbedTextOut);
        return Library::Call< LONG, HDC, int, int, LPCTSTR, int, int, CONST ::LPINT, int >(pFunc, hDC, X, Y, lpString, nCount, nTabPositions, lpnTabStopPositions, nTabOrigin);
    }

}}}//qor::win::api
