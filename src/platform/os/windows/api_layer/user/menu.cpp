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
    //MenuItemFromPoint), 1 DWORD (CheckMenuItem), 1 LONG (GetMenuCheckMarkDimensions). Three parameter types do diverge,
    //and all three are published header surface, so they are recorded (U18) and not changed in a TU session:
    //  - TrackPopupMenu's last parameter is HWND prcRect (:341, user32.h:252) where the SDK has _Reserved_ CONST RECT*
    //    (winuser.h:7759-7766). STRICT is on in this build (winnt.h defines it unless NO_STRICT - measured in session 17),
    //    so HWND is struct HWND__* and a RECT* will not convert implicitly: the slot can carry only NULL or a cast.
    //    Pointer-sized on both architectures, so no ABI effect and no UB - and the wrapper has no caller at all (below).
    //  - InsertMenuT (:250) and ModifyMenuT (:292) take void* uIDNewItem where the SDK takes UINT_PTR (winuser.h:7658,
    //    :7667, :7704, :7713). AppendMenuT (:82) in the same section spells it correctly, so the section contradicts
    //    itself. The SDK's UINT_PTR exists so one slot can carry either an item id or a string pointer; void* forces a
    //    cast on the id case, and the two live callers pass it through (ui/gui/controllers/i_m/menu.cpp:173, :183).
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

    DWORD User32::CheckMenuItem(HMENU hmenu, UINT uIDCheckItem, UINT uCheck)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CheckMenuItem);
        return Library::Call<DWORD, HMENU, UINT, UINT>(pFunc, hmenu, uIDCheckItem, uCheck);
    }

    BOOL User32::CheckMenuRadioItem(HMENU hmenu, UINT idFirst, UINT idLast, UINT idCheck, UINT uFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CheckMenuRadioItem);
        return Library::Call<BOOL, HMENU, UINT, UINT, UINT, UINT>(pFunc, hmenu, idFirst, idLast, idCheck, uFlags);
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

    BOOL User32::GetMenuItemInfoT(HMENU hMenu, UINT uItem, BOOL fByPosition, LPMENUITEMINFO lpmii)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetMenuItemInfo);
        return Library::Call<BOOL, HMENU, UINT, BOOL, LPMENUITEMINFO>(pFunc, hMenu, uItem, fByPosition, lpmii);
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

    int User32::GetMenuStringT(HMENU hMenu, UINT uIDItem, LPTSTR lpString, int nMaxCount, UINT uFlag)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetMenuString);
        return Library::Call<int, HMENU, UINT, LPTSTR, int, UINT>(pFunc, hMenu, uIDItem, lpString, nMaxCount, uFlag);
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

    BOOL User32::HiliteMenuItem(HWND hwnd, HMENU hmenu, UINT uItemHilite, UINT uHilite)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, HiliteMenuItem);
        return Library::Call<BOOL, HWND, HMENU, UINT, UINT>(pFunc, hwnd, hmenu, uItemHilite, uHilite);
    }

    BOOL User32::InsertMenuT(HMENU hMenu, UINT uPosition, UINT uFlags, void* uIDNewItem, LPCTSTR lpNewItem)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, InsertMenu);
        return Library::Call<BOOL, HMENU, UINT, UINT, void*, LPCTSTR>(pFunc, hMenu, uPosition, uFlags, uIDNewItem, lpNewItem);
    }

    BOOL User32::InsertMenuItemT(HMENU hMenu, UINT uItem, BOOL fByPosition, LPCMENUITEMINFO lpmii)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, InsertMenuItem);
        return Library::Call<BOOL, HMENU, UINT, BOOL, LPCMENUITEMINFO>(pFunc, hMenu, uItem, fByPosition, lpmii);
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

    HMENU User32::LoadMenuIndirectT(CONST MENUTEMPLATE* lpMenuTemplate)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, LoadMenuIndirect);
        return Library::Call<HMENU, CONST MENUTEMPLATE*>(pFunc, lpMenuTemplate);
    }

    int User32::MenuItemFromPoint(HWND hWnd, HMENU hMenu, POINT ptScreen)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, MenuItemFromPoint);
        return Library::Call<int, HWND, HMENU, POINT>(pFunc, hWnd, hMenu, ptScreen);
    }

    BOOL User32::ModifyMenuT(HMENU hMenu, UINT uPosition, UINT uFlags, void* uIDNewItem, LPCTSTR lpNewItem)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, ModifyMenu);
        return Library::Call<BOOL, HMENU, UINT, UINT, void*, LPCTSTR>(pFunc, hMenu, uPosition, uFlags, uIDNewItem, lpNewItem);
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

    BOOL User32::SetMenuItemInfoT(HMENU hMenu, UINT uItem, BOOL fByPosition, LPMENUITEMINFO lpmii)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, SetMenuItemInfo);
        return Library::Call<BOOL, HMENU, UINT, BOOL, LPMENUITEMINFO>(pFunc, hMenu, uItem, fByPosition, lpmii);
    }

    BOOL User32::TrackPopupMenu(HMENU hMenu, UINT uFlags, int x, int y, int nReserved, HWND hWnd, HWND prcRect)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, TrackPopupMenu);
        return Library::Call<BOOL, HMENU, UINT, int, int, int, HWND, HWND>(pFunc, hMenu, uFlags, x, y, nReserved, hWnd, prcRect);
    }

    BOOL User32::TrackPopupMenuEx(HMENU hmenu, UINT fuFlags, int x, int y, HWND hwnd, ::LPTPMPARAMS lptpm)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, TrackPopupMenuEx);
        return Library::Call<BOOL, HMENU, UINT, int, int, HWND, ::LPTPMPARAMS>(pFunc, hmenu, fuFlags, x, y, hwnd, lptpm);
    }

}}}//qor::win::api
