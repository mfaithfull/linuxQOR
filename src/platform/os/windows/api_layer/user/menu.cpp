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
    //Thirty-nine wrappers - the whole "user32.h:214 //Menu functions" section, declared :215-253 and defined here in
    //header order (:82 AppendMenuT .. :348 TrackPopupMenuEx). All 39 declarations are static and all 39 are defined here,
    //so 0 U4 and 0 U13 instances, and the decl-vs-def signature compare over the section gives 0 flags
    //(tmp/_tmp_menu_pre_s22.txt).
    //
    //Binding (D13): nine stems have an SDK A/W family and bind through qor_pp_useswinapiAW (AppendMenu, GetMenuItemInfo,
    //GetMenuString, InsertMenu, InsertMenuItem, LoadMenu, LoadMenuIndirect, ModifyMenu, SetMenuItemInfo); the other thirty
    //bind bare, and each of those thirty is a genuine single export - tmp/_tmp_menu_class_s22.ps1 checks every stem against
    //the export table (tmp/_tmp_user32_exports_s22.txt, 1018 names) and against winuser.h, and for all thirty the bare name
    //is exported while no <stem>A/<stem>W spelling exists and winuser.h defines no "<stem> -> <stem>W" macro, so bare is
    //the only spelling that could ever resolve. The nine AW stems are exactly the nine with a live winuser.h macro, and no
    //bare spelling of those nine is an export, so bare there would be a run-time throw (G1/G2). Two of the nine were bare
    //until this session - GetMenuString (:225) and InsertMenuItem (:260), the pre-scan P2 pair. That class is latent, not
    //broken: the SDK macro rewrote the bare stem before it was stringized, so cl /EP emitted "GetMenuStringW" and
    //"InsertMenuItemW" before the change and the same two literals after it, now reached by ##W pasting (kernel32.h:52)
    //instead of by the SDK macro - behaviour-neutral, as with dialog.cpp's MessageBox (session 13). Post-fix this TU holds
    //0 of the folder's unresolvable bindings: the folder census goes 17 -> 15 (tmp/_tmp_p1census_post_s22.txt) with no
    //menu.cpp line left in it, and cl /EP over this TU yields 39 literals, all 39 in the export table.
    //
    //Signatures: 0 return-type divergences. tmp/_tmp_menu_ret_s22.ps1 reads the SDK return type off its own line above the
    //stem, and all 39 agree - 24 BOOL, 7 HMENU (CreateMenu, CreatePopupMenu, GetMenu, GetSubMenu, GetSystemMenu, LoadMenu,
    //LoadMenuIndirect), 3 UINT (GetMenuDefaultItem, GetMenuItemID, GetMenuState), 3 int (GetMenuItemCount, GetMenuString,
    //MenuItemFromPoint), 1 DWORD (CheckMenuItem), 1 LONG (GetMenuCheckMarkDimensions). The three parameter types that
    //used to diverge are fixed (T8 steps 1-3, 6 Nov 2026):
    //  - TrackPopupMenu (:340) now spells the SDK's own seven parameters - int nReserved plus const RECT* prcRect
    //    (winuser.h:7759-7766) - instead of the HWND prcRect slot this file declared before STRICT made that slot
    //    uncarriable except as NULL.
    //  - InsertMenuT (:250) and ModifyMenuT (:292) now take UINT_PTR uIDNewItem as the SDK does (winuser.h:7658,
    //    :7667, :7704, :7713) and as AppendMenuT (:82) always did, so the section no longer contradicts itself. The two
    //    live callers (ui/gui/controllers/i_m/menu.cpp:173, :183) pass a pointer, so each now casts
    //    reinterpret_cast<UINT_PTR>; the GUI header cannot name UINT_PTR at all (it is parsed before the Windows
    //    headers), which is why the cast sits at the call site and not in Menu::InsertSubMenu / Menu::Modify.
    //Parameter names here are the SDK's since the folder-wide alignment of the same day (U18 step 4): 21 names over 9
    //members moved, hmenu -> hMenu at CheckMenuItem and TrackPopupMenuEx, hMenu -> hmenu at GetMenuItemInfoT,
    //InsertMenuItemT and SetMenuItemInfoT (the SDK really does spell it that way there), and fByPosition -> fByPositon,
    //the SDK's own typo at winuser.h:8015 - the rule is the prototype, typo included.
    //Each Call<> list restates its member's published types, so the lists at :254 and :296 inherit the divergence: fixing a
    //signature here means fixing the list too, which is the U23 shape.
    //
    //Call sites: 35 of 39 wrappers have a LAYER call site - tmp/_tmp_menu_bypass_s22.txt counts 40 LAYER, 10 INDIRECT,
    //18 BYPASS rows and 6 MEMBER, concentrated in ui/gui/controllers/i_m/menu.cpp:40-208 (the Menu controller),
    //n_r/popupmenu.cpp:40 and :58, and windowcontroller.cpp:466-486. The four with no call site at all are U15:
    //GetMenuCheckMarkDimensions, IsMenu, LoadMenuT and TrackPopupMenu. Of the 18 BYPASS rows, 3 are controller
    //*declarations* that shadow the SDK stem (i_m/menu.h:95, windowcontroller.h:226, :229) and are not calls; the other
    //15 are all in examples/experimental/osspecific/windows/retropad/retropad.cpp, whose subdirectory is commented out at
    //examples/experimental/osspecific/windows/CMakeLists.txt:8, so the menu family has 0 bypass sites in built code - the
    //cursor.cpp result (session 9), not the clipboard.cpp one (session 5).
    //
    //One SDK family, three header sections (U21, fourth instance): the SDK's menu family is the #ifndef NOMENUS region
    //winuser.h:7432-8101 (38 distinct stems) plus a lone CheckMenuRadioItem at :10509 inside a second NOMENUS region
    //(:10426-:10535), which is why that one pairs with a WINVER >= 0x0400 guard rather than with the NOMENUS
    //neighbourhood. This section owns 36 of the region's 38 plus that lone one; two of its own members pair with SDK
    //declarations outside NOMENUS - IsMenu (winuser.h:4492) and GetMenuBarInfo (:14740, under NOWINABLE). ChangeMenuA/W
    //(:7481, :7485) and CalculatePopupWindowPosition (:8073) sit in the SDK's menu region and are declared nowhere in this
    //library, and the family's context-help pair is declared in the header's "//Shell functions" block (:642-643) - the T4
    //pair, declared non-static where its Window neighbours are static, defined in no TU, although both are real exports
    //(ordinals 34A and 17C).
    //
    //Version guards (U16): 12 of the 39 pair with SDK declarations the SDK emits only if a WINVER check passes -
    //TrackPopupMenuEx, InsertMenuItem, GetMenuItemInfo, SetMenuItemInfo, GetMenuDefaultItem, SetMenuDefaultItem,
    //GetMenuItemRect, MenuItemFromPoint, CheckMenuRadioItem at WINVER >= 0x0400 and GetMenuInfo, SetMenuInfo, EndMenu at
    //WINVER >= 0x0500 (tmp/_tmp_menu_guardsum_s22.txt). Nothing in the build pins WINVER, so those prototypes are visible
    //only because sdkddkver.h defaults it to 0x0A00. The nine A/W members are also the nine whose parameter types follow
    //the UNICODE switch (LPCTSTR/LPTSTR, LPMENUITEMINFO/LPCMENUITEMINFO, MENUTEMPLATE) while D13 now forces the W entry
    //point by ##W - the U26 include-order hazard, live in this TU.
    //
    //Not touched, by policy: no Win32ErrorHandler and no CheckReturn here (the session-3 decision, U1/U2). This family is
    //the sharpest case for the end-of-review pass, because failure is often reported in band rather than as FALSE/NULL -
    //the SDK documents -1 from GetMenuItemCount and MenuItemFromPoint and 0xFFFFFFFF from GetMenuState and
    //GetMenuDefaultItem - while 24 of the 39 wrappers return BOOL and none of it is looked at.

    BOOL User32::AppendMenuT(HMENU hMenu, UINT uFlags, UINT_PTR uIDNewItem, LPCTSTR lpNewItem)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, AppendMenu);
        return Library::Call<BOOL, HMENU, UINT, UINT_PTR, LPCTSTR>(pFunc, hMenu, uFlags, uIDNewItem, lpNewItem);
    }

    DWORD User32::CheckMenuItem(HMENU hMenu, UINT uIDCheckItem, UINT uCheck)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CheckMenuItem);
        return Library::Call<DWORD, HMENU, UINT, UINT>(pFunc, hMenu, uIDCheckItem, uCheck);
    }

    BOOL User32::CheckMenuRadioItem(HMENU hmenu, UINT first, UINT last, UINT check, UINT flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CheckMenuRadioItem);
        return Library::Call<BOOL, HMENU, UINT, UINT, UINT, UINT>(pFunc, hmenu, first, last, check, flags);
    }

    HMENU User32::CreateMenu(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CreateMenu);
        return Library::Call<HMENU>(pFunc);
    }

    HMENU User32::CreatePopupMenu(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CreatePopupMenu);
        return Library::Call<HMENU>(pFunc);
    }

    BOOL User32::DeleteMenu(HMENU hMenu, UINT uPosition, UINT uFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DeleteMenu);
        return Library::Call<BOOL, HMENU, UINT, UINT>(pFunc, hMenu, uPosition, uFlags);
    }

    BOOL User32::DestroyMenu(HMENU hMenu)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DestroyMenu);
        return Library::Call<BOOL, HMENU>(pFunc, hMenu);
    }

    BOOL User32::DrawMenuBar(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, DrawMenuBar);
        return Library::Call<BOOL, HWND>(pFunc, hWnd);
    }

    BOOL User32::EnableMenuItem(HMENU hMenu, UINT uIDEnableItem, UINT uEnable)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EnableMenuItem);
        return Library::Call<BOOL, HMENU, UINT, UINT>(pFunc, hMenu, uIDEnableItem, uEnable);
    }

    BOOL User32::EndMenu(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EndMenu);
        return Library::Call<BOOL>(pFunc);
    }

    HMENU User32::GetMenu(HWND hWnd)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMenu);
        return Library::Call<HMENU, HWND>(pFunc, hWnd);
    }

    BOOL User32::GetMenuBarInfo(HWND hwnd, LONG idObject, LONG idItem, PMENUBARINFO pmbi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMenuBarInfo);
        return Library::Call<BOOL, HWND, LONG, LONG, PMENUBARINFO>(pFunc, hwnd, idObject, idItem, pmbi);
    }

    LONG User32::GetMenuCheckMarkDimensions(void)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMenuCheckMarkDimensions);
        return Library::Call<LONG>(pFunc);
    }

    UINT User32::GetMenuDefaultItem(HMENU hMenu, UINT fByPos, UINT gmdiFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMenuDefaultItem);
        return Library::Call<UINT, HMENU, UINT, UINT>(pFunc, hMenu, fByPos, gmdiFlags);
    }

    BOOL User32::GetMenuInfo(HMENU hmenu, ::LPCMENUINFO lpcmi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMenuInfo);
        return Library::Call<BOOL, HMENU, ::LPCMENUINFO>(pFunc, hmenu, lpcmi);
    }

    int User32::GetMenuItemCount(HMENU hMenu)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMenuItemCount);
        return Library::Call<int, HMENU>(pFunc, hMenu);
    }

    UINT User32::GetMenuItemID(HMENU hMenu, int nPos)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMenuItemID);
        return Library::Call<UINT, HMENU, int>(pFunc, hMenu, nPos);
    }

    BOOL User32::GetMenuItemInfoT(HMENU hmenu, UINT item, BOOL fByPosition, LPMENUITEMINFO lpmii)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetMenuItemInfo);
        return Library::Call<BOOL, HMENU, UINT, BOOL, LPMENUITEMINFO>(pFunc, hmenu, item, fByPosition, lpmii);
    }

    BOOL User32::GetMenuItemRect(HWND hWnd, HMENU hMenu, UINT uItem, ::LPRECT lprcItem)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMenuItemRect);
        return Library::Call<BOOL, HWND, HMENU, UINT, ::LPRECT>(pFunc, hWnd, hMenu, uItem, lprcItem);
    }

    UINT User32::GetMenuState(HMENU hMenu, UINT uId, UINT uFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetMenuState);
        return Library::Call<UINT, HMENU, UINT, UINT>(pFunc, hMenu, uId, uFlags);
    }

    int User32::GetMenuStringT(HMENU hMenu, UINT uIDItem, LPTSTR lpString, int cchMax, UINT flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetMenuString);
        return Library::Call<int, HMENU, UINT, LPTSTR, int, UINT>(pFunc, hMenu, uIDItem, lpString, cchMax, flags);
    }

    HMENU User32::GetSubMenu(HMENU hMenu, int nPos)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetSubMenu);
        return Library::Call<HMENU, HMENU, int>(pFunc, hMenu, nPos);
    }

    HMENU User32::GetSystemMenu(HWND hWnd, BOOL bRevert)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, GetSystemMenu);
        return Library::Call<HMENU, HWND, BOOL>(pFunc, hWnd, bRevert);
    }

    BOOL User32::HiliteMenuItem(HWND hWnd, HMENU hMenu, UINT uIDHiliteItem, UINT uHilite)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, HiliteMenuItem);
        return Library::Call<BOOL, HWND, HMENU, UINT, UINT>(pFunc, hWnd, hMenu, uIDHiliteItem, uHilite);
    }

    BOOL User32::InsertMenuT(HMENU hMenu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, LPCTSTR lpNewItem)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, InsertMenu);
        return Library::Call<BOOL, HMENU, UINT, UINT, UINT_PTR, LPCTSTR>(pFunc, hMenu, uPosition, uFlags, uIDNewItem, lpNewItem);
    }

    BOOL User32::InsertMenuItemT(HMENU hmenu, UINT item, BOOL fByPosition, LPCMENUITEMINFO lpmi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, InsertMenuItem);
        return Library::Call<BOOL, HMENU, UINT, BOOL, LPCMENUITEMINFO>(pFunc, hmenu, item, fByPosition, lpmi);
    }

    BOOL User32::IsMenu(HMENU hMenu)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsMenu);
        return Library::Call<BOOL, HMENU>(pFunc, hMenu);
    }

    HMENU User32::LoadMenuT(HINSTANCE hInstance, LPCTSTR lpMenuName)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, LoadMenu);
        return Library::Call<HMENU, HINSTANCE, LPCTSTR>(pFunc, hInstance, lpMenuName);
    }

    HMENU User32::LoadMenuIndirectT(const MENUTEMPLATE* lpMenuTemplate)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, LoadMenuIndirect);
        return Library::Call<HMENU, const MENUTEMPLATE*>(pFunc, lpMenuTemplate);
    }

    int User32::MenuItemFromPoint(HWND hWnd, HMENU hMenu, POINT ptScreen)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, MenuItemFromPoint);
        return Library::Call<int, HWND, HMENU, POINT>(pFunc, hWnd, hMenu, ptScreen);
    }

    BOOL User32::ModifyMenuT(HMENU hMnu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, LPCTSTR lpNewItem)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, ModifyMenu);
        return Library::Call<BOOL, HMENU, UINT, UINT, UINT_PTR, LPCTSTR>(pFunc, hMnu, uPosition, uFlags, uIDNewItem, lpNewItem);
    }

    BOOL User32::RemoveMenu(HMENU hMenu, UINT uPosition, UINT uFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, RemoveMenu);
        return Library::Call<BOOL, HMENU, UINT, UINT>(pFunc, hMenu, uPosition, uFlags);
    }

    BOOL User32::SetMenu(HWND hWnd, HMENU hMenu)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetMenu);
        return Library::Call<BOOL, HWND, HMENU>(pFunc, hWnd, hMenu);
    }

    BOOL User32::SetMenuDefaultItem(HMENU hMenu, UINT uItem, UINT fByPos)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetMenuDefaultItem);
        return Library::Call<BOOL, HMENU, UINT, UINT>(pFunc, hMenu, uItem, fByPos);
    }

    BOOL User32::SetMenuInfo(HMENU hmenu, ::LPCMENUINFO lpcmi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetMenuInfo);
        return Library::Call<BOOL, HMENU, ::LPCMENUINFO>(pFunc, hmenu, lpcmi);
    }

    BOOL User32::SetMenuItemBitmaps(HMENU hMenu, UINT uPosition, UINT uFlags, HBITMAP hBitmapUnchecked, HBITMAP hBitmapChecked)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetMenuItemBitmaps);
        return Library::Call<BOOL, HMENU, UINT, UINT, HBITMAP, HBITMAP>(pFunc, hMenu, uPosition, uFlags, hBitmapUnchecked, hBitmapChecked);
    }

    BOOL User32::SetMenuItemInfoT(HMENU hmenu, UINT item, BOOL fByPositon, LPMENUITEMINFO lpmii)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetMenuItemInfo);
        return Library::Call<BOOL, HMENU, UINT, BOOL, LPMENUITEMINFO>(pFunc, hmenu, item, fByPositon, lpmii);
    }

    BOOL User32::TrackPopupMenu(HMENU hMenu, UINT uFlags, int x, int y, int nReserved, HWND hWnd, const RECT* prcRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, TrackPopupMenu);
        return Library::Call<BOOL, HMENU, UINT, int, int, int, HWND, const RECT*>(pFunc, hMenu, uFlags, x, y, nReserved, hWnd, prcRect);
    }

    BOOL User32::TrackPopupMenuEx(HMENU hMenu, UINT uFlags, int x, int y, HWND hwnd, ::LPTPMPARAMS lptpm)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, TrackPopupMenuEx);
        return Library::Call<BOOL, HMENU, UINT, int, int, HWND, ::LPTPMPARAMS>(pFunc, hMenu, uFlags, x, y, hwnd, lptpm);
    }

}}}//qor::win::api
