// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "../kernel/errorhandler.h"

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

    //Return policy (U2, table row AppendMenu / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - the menu being appended to
    //and the item's text are parameters, so the BOOL carries no answer and zero is reserved; the row's sdkRetType is
    //NO-PROTOTYPE (AW binding) so the base is the published BOOL and BoolCheck (returncheck.h:94) covers it; the page is
    //reached through the suffixed ANSI twin slug nf-winuser-appendmenua (CORRECTED leg). AUDIT: the table counts 0 in-tree
    //callers for the bare stem, but this wrapper's spelling is AppendMenuT and it is called at i_m/menu.cpp :77 :84 :93 -
    //each site already tests the return (... ? true : false and if(...)), so a fired check adds only the continuable
    //diagnostic and each site's own false / -1 branch is unchanged.
    BOOL User32::AppendMenuT(HMENU hMenu, UINT uFlags, UINT_PTR uIDNewItem, LPCTSTR lpNewItem)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, AppendMenu);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HMENU, UINT, UINT_PTR, LPCTSTR>(pFunc, hMenu, uFlags, uIDNewItem, lpNewItem);
        return bResult;
    }

    //Return policy (U2, table row CheckMenuItem / menu.cpp, 12 Nov 2026): the row reads checked with checkClass
    //TCheckFailureValue< DWORD, ( DWORD )-1 >, so the call carries the token - docQuote "Type: DWORD The return value
    //specifies the previous state of the menu item (either MF_CHECKED or MF_UNCHECKED ). If the menu item does not exist,
    //the return value is UINT_MAX." - the return is the previous check state the caller asked about, so one of the two flag
    //values (MF_UNCHECKED) is zero and CheckNonZero< DWORD > would report a legal unchecked item as a failure; the only
    //reserved value is UINT_MAX, which is ( DWORD )-1, so TCheckFailureValue (returncheck.h:213, Test fails only at iFailure)
    //passes at both flag values; the same form the rawinput.cpp rows already carry. AUDIT: the table counts 1 in-tree caller
    //and the site-level read is i_m/menu.cpp:67 "return User32::CheckMenuItem((HMENU)(m_handle.Use()), uIDCheckItem, uCheck);"
    //into "unsigned long Menu::CheckItem(...)" - the site forwards the state as its own answer, so the check fires only at
    //the documented nonexistent-item sentinel and both flag values pass through unchanged.
    DWORD User32::CheckMenuItem(HMENU hMenu, UINT uIDCheckItem, UINT uCheck)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CheckMenuItem);
        CheckReturn< DWORD, TCheckFailureValue< DWORD, ( DWORD )-1 > >::TType dResult = Library::Call<DWORD, HMENU, UINT, UINT>(pFunc, hMenu, uIDCheckItem, uCheck);
        return dResult;
    }

    //Return policy (U2, table row CheckMenuRadioItem / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //so the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, use the GetLastError function." - the radio
    //group and its indices are parameters, so the BOOL carries no answer and zero is reserved; sdkRetType BOOL so BoolCheck
    //(returncheck.h:94) covers it. AUDIT: the table counts 1 in-tree caller and the site-level read is i_m/menu.cpp:98
    //"return User32::CheckMenuRadioItem(MENU_HANDLE, idFirst, idLast, idCheck, flags) ? true : false;" - the site already
    //collapses the BOOL, so a fired check adds only the diagnostic and the site's false branch is unchanged.
    BOOL User32::CheckMenuRadioItem(HMENU hmenu, UINT first, UINT last, UINT check, UINT flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CheckMenuRadioItem);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HMENU, UINT, UINT, UINT, UINT>(pFunc, hmenu, first, last, check, flags);
        return bResult;
    }

    //Return policy (U2, table row CreateMenu / menu.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HMENU >, so the call carries the token - docQuote "Type: HMENU If the function succeeds, the return
    //value is a handle to the newly created menu. If the function fails, the return value is NULL . To get extended error
    //information, call GetLastError ." - the handle is what the caller hands to AddMenu/AppendMenu next, so a NULL menu handle
    //is never usable and zero is reserved; the row's sdkRetType column is blank so the class base is the published HMENU, and
    //CheckNonZeroHandle (returncheck.h:137, template< typename THandle >) covers it - the same shape the pointer.cpp
    //CreateSyntheticPointerDevice row carries. AUDIT: the table counts 1 in-tree caller and the site-level read is menu.cpp:40
    //"Menu::Menu() : m_handle(User32::CreateMenu())" - the handle is stored in the controller's m_handle member, so a fired
    //check adds only the continuable diagnostic and the member keeps whatever the call returned; the check fires only at the
    //documented NULL.
    HMENU User32::CreateMenu(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CreateMenu);
        CheckReturn< HMENU, CheckNonZeroHandle< HMENU > >::TType hResult = Library::Call<HMENU>(pFunc);
        return hResult;
    }

    //Return policy (U2, table row CreatePopupMenu / menu.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HMENU >, so the call carries the token - docQuote "Type: HMENU If the function succeeds, the return
    //value is a handle to the newly created menu. If the function fails, the return value is NULL . To get extended error
    //information, call GetLastError ." - the CreateMenu row's Return value section word for word (the popup twin), so a NULL
    //menu handle is never usable and zero is reserved; the row's sdkRetType column is blank so the class base is the published
    //HMENU and CheckNonZeroHandle (returncheck.h:137) fits. AUDIT: the table counts 1 in-tree caller and the site-level read
    //is n_r/popupmenu.cpp:40 "PopupMenu::PopupMenu() : Menu(User32::CreatePopupMenu())" - the handle initialises the Menu
    //member, so a fired check adds only the continuable diagnostic and the member keeps whatever the call returned; the check
    //fires only at the documented NULL.
    HMENU User32::CreatePopupMenu(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CreatePopupMenu);
        CheckReturn< HMENU, CheckNonZeroHandle< HMENU > >::TType hResult = Library::Call<HMENU>(pFunc);
        return hResult;
    }

    //Return policy (U2, table row DeleteMenu / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - the menu and item id are
    //parameters, so the BOOL carries no answer and zero is reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers
    //it. AUDIT: the table counts 1 in-tree caller and the site-level read is menu.cpp:103
    //"return User32::DeleteMenu(MENU_HANDLE, position, flags) ? true : false;" - the site already collapses the BOOL, so a
    //fired check adds only the diagnostic and the site's false branch is unchanged.
    BOOL User32::DeleteMenu(HMENU hMenu, UINT uPosition, UINT uFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DeleteMenu);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HMENU, UINT, UINT>(pFunc, hMenu, uPosition, uFlags);
        return bResult;
    }

    //Return policy (U2, table row DestroyMenu / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - the DeleteMenu row's wording
    //word for word, the menu handle is a parameter, so the BOOL carries no answer and zero is reserved; sdkRetType BOOL so
    //BoolCheck (returncheck.h:94) covers it. AUDIT: the table counts 1 in-tree caller and the site-level read is menu.cpp:108
    //"return User32::DestroyMenu(MENU_HANDLE) ? true : false;" - the site already collapses the BOOL, so a fired check adds
    //only the diagnostic and the site's false branch is unchanged.
    BOOL User32::DestroyMenu(HMENU hMenu)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DestroyMenu);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HMENU>(pFunc, hMenu);
        return bResult;
    }

    //Return policy (U2, table row DrawMenuBar / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - the window whose bar is drawn
    //is a parameter and the drawing itself returns nothing, so the BOOL carries no answer and zero is reserved; sdkRetType
    //BOOL so BoolCheck (returncheck.h:94) covers it. AUDIT: the table counts 1 in-tree caller and the site-level read is
    //windowcontroller.cpp:468 "return User32::DrawMenuBar(WINDOW_HANDLE) ? true :false;" - the site already collapses the
    //BOOL, so a fired check adds only the diagnostic and the site's false branch is unchanged.
    BOOL User32::DrawMenuBar(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, DrawMenuBar);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND>(pFunc, hWnd);
        return bResult;
    }

    //Return policy (U2, table row EnableMenuItem / menu.cpp, 12 Nov 2026): the row reads checked with checkClass
    //TCheckFailureValue< BOOL, -1 >, so the call carries the token - docQuote "Type: BOOL The return value specifies the
    //previous state of the menu item (it is either MF_DISABLED , MF_ENABLED , or MF_GRAYED ). If the menu item does not exist,
    //the return value is -1." - the return is the previous enable state the caller asked about, so one of the three flag
    //values is zero and keying CheckNonZero< BOOL > or BoolCheck would report a legal MF_ENABLED item as a failure; -1 is
    //the only reserved value, so TCheckFailureValue< BOOL, -1 > (returncheck.h:213) passes at all three flag values; the
    //published BOOL row type is typedef int in the Windows headers this layer includes, so -1 survives the comparison - the
    //same reading the GetMessage row carries. AUDIT: the table counts 1 in-tree caller and the site-level read is menu.cpp:72
    //"return User32::EnableMenuItem((HMENU)(m_handle.Use()), uIDEnableItem, uEnable) ? true : false;" - the site already
    //collapses the value into a BOOL, so a fired check adds only the diagnostic and the site's false branch is unchanged.
    BOOL User32::EnableMenuItem(HMENU hMenu, UINT uIDEnableItem, UINT uEnable)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EnableMenuItem);
        CheckReturn< BOOL, TCheckFailureValue< BOOL, -1 > >::TType bResult = Library::Call<BOOL, HMENU, UINT, UINT>(pFunc, hMenu, uIDEnableItem, uEnable);
        return bResult;
    }

    //Return policy (U2, table row EndMenu / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so the
    //call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - the DeleteMenu row's wording
    //word for word, the window is a parameter, so the BOOL carries no answer and zero is reserved; sdkRetType BOOL so BoolCheck
    //(returncheck.h:94) covers it. AUDIT: the table counts 1 in-tree caller and the site-level read is menu.cpp:113
    //"return User32::EndMenu() ? true : false;" - the site already collapses the BOOL, so a fired check adds only the diagnostic
    //and the site's false branch is unchanged.
    BOOL User32::EndMenu(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EndMenu);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL>(pFunc);
        return bResult;
    }

    HMENU User32::GetMenu(HWND hWnd)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetMenu);
        return Library::Call<HMENU, HWND>(pFunc, hWnd);
    }

    //Return policy (U2, table row GetMenuBarInfo / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - the MENU_BARINFO the caller
    //wants is an out-parameter and the ids are parameters, so the BOOL carries no answer and zero is reserved; sdkRetType
    //BOOL so BoolCheck (returncheck.h:94) covers it. AUDIT: the table counts 1 in-tree caller and the site-level read is
    //windowcontroller.cpp:481 "return User32::GetMenuBarInfo(WINDOW_HANDLE, idObject, idItem, reinterpret_cast<PMENUBARINFO>(&mbi)) ? true : false;" - the site already collapses the BOOL, so a fired check adds only the diagnostic and the site's false branch is unchanged.
    BOOL User32::GetMenuBarInfo(HWND hwnd, LONG idObject, LONG idItem, PMENUBARINFO pmbi)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetMenuBarInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, LONG, LONG, PMENUBARINFO>(pFunc, hwnd, idObject, idItem, pmbi);
        return bResult;
    }

    LONG User32::GetMenuCheckMarkDimensions(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetMenuCheckMarkDimensions);
        return Library::Call<LONG>(pFunc);
    }

    //Return policy (U2, table row GetMenuDefaultItem / menu.cpp, 12 Nov 2026): the row reads checked with checkClass
    //TCheckFailureValue< UINT, ( UINT )-1 >, so the call carries the token - docQuote "Type: UINT If the function succeeds,
    //the return value is the identifier or position of the menu item. If the function fails, the return value is -1. To get
    //extended error information, call GetLastError ." - the item's position or identifier is the answer, and position 0 (the
    //first item) is legal, so keying CheckNonZero< UINT > would report a legal first item as a failure; -1 is the only reserved
    //value, and for the UINT row type that is ( UINT )-1, so TCheckFailureValue< UINT, ( UINT )-1 > (returncheck.h:213) passes
    //at every legal position; the same form the rawinput.cpp UINT rows already carry. AUDIT: the table counts 1 in-tree caller
    //and the site-level read is menu.cpp:118 "return User32::GetMenuDefaultItem(MENU_HANDLE, byPos, flags);" - the site forwards
    //the item as its own answer, so a fired check adds only the continuable diagnostic and both legal positions pass through unchanged.
    UINT User32::GetMenuDefaultItem(HMENU hMenu, UINT fByPos, UINT gmdiFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetMenuDefaultItem);
        CheckReturn< UINT, TCheckFailureValue< UINT, ( UINT )-1 > >::TType uResult = Library::Call<UINT, HMENU, UINT, UINT>(pFunc, hMenu, fByPos, gmdiFlags);
        return uResult;
    }

    //Return policy (U2, table row GetMenuInfo / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - a Get*Info whose payload is an
    //out-parameter: the MENUINFO structure the caller fills is an out-parameter, so the BOOL carries no answer and zero is
    //reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. AUDIT: the table counts 1 in-tree caller and the
    //site-level read is menu.cpp:123 "return User32::GetMenuInfo(MENU_HANDLE, reinterpret_cast<LPCMENUINFO>(&menuInfo)) ? true : false;" - the site already collapses the BOOL, so a fired check adds only the diagnostic and the site's false branch is unchanged.
    BOOL User32::GetMenuInfo(HMENU hmenu, ::LPCMENUINFO lpcmi)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetMenuInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HMENU, ::LPCMENUINFO>(pFunc, hmenu, lpcmi);
        return bResult;
    }

    //Return policy (U2, table row GetMenuItemCount / menu.cpp, 12 Nov 2026): the row reads checked with checkClass
    //TCheckFailureValue< int, -1 >, so the call carries the token - docQuote "Type: int If the function succeeds, the return
    //value specifies the number of items in the menu. If the function fails, the return value is -1. To get extended error
    //information, call GetLastError ." - a count with a reserved sentinel that is not zero: a menu with no items is a legal
    //state answering 0, so keying CheckNonZero< int > would report a legal empty menu as a failure while -1 is the documented
    //failure; TCheckFailureValue< int, -1 > (returncheck.h:213) passes at every count; sdkRetType int. AUDIT: the table counts
    //1 in-tree caller and the site-level read is menu.cpp:128 "return User32::GetMenuItemCount(MENU_HANDLE);" - the site forwards
    //the count as its own answer, so a fired check adds only the continuable diagnostic and every count passes through unchanged.
    int User32::GetMenuItemCount(HMENU hMenu)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetMenuItemCount);
        CheckReturn< int, TCheckFailureValue< int, -1 > >::TType iResult = Library::Call<int, HMENU>(pFunc, hMenu);
        return iResult;
    }

    //Return policy (U2, table row GetMenuItemID / menu.cpp, 12 Nov 2026): the row reads checked with checkClass
    //TCheckFailureValue< UINT, ( UINT )-1 >, so the call carries the token - docQuote "Type: UINT The return value is the
    //identifier of the specified menu item. If the menu item identifier is NULL or if the specified item opens a submenu, the
    //return value is -1." - an identifier answer with a reserved sentinel that is not zero: identifiers start at 0 for the first
    //item, so a zero identifier is a legal answer and only -1 is reserved; TCheckFailureValue< UINT, ( UINT )-1 > (returncheck.h:213)
    //covers the UINT row type; deliberately the -1 here also answers "the item opens a submenu", a legal question about the item's
    //shape, which is recorded in the reason rather than decided as a second failure value. AUDIT: the table counts 1 in-tree caller
    //and the site-level read is menu.cpp:133 "return User32::GetMenuItemID(MENU_HANDLE, pos);" - the site forwards the identifier,
    //so a fired check adds only the continuable diagnostic and every identifier passes through unchanged.
    UINT User32::GetMenuItemID(HMENU hMenu, int nPos)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetMenuItemID);
        CheckReturn< UINT, TCheckFailureValue< UINT, ( UINT )-1 > >::TType uResult = Library::Call<UINT, HMENU, int>(pFunc, hMenu, nPos);
        return uResult;
    }

    //Return policy (U2, table row GetMenuItemInfo / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, use the GetLastError function." - a plain status BOOL
    //whose payload is an out-parameter: the MENUITEMINFO structure the caller fills is an out-parameter, so the BOOL carries no
    //answer and zero is reserved; the row's sdkRetType is NO-PROTOTYPE (AW binding) so the base is the published BOOL and BoolCheck
    //(returncheck.h:94) covers it; the page is reached through the suffixed ANSI twin slug nf-winuser-getmenuiteminfoa (CORRECTED leg). AUDIT: the table counts 0 in-tree callers (change-safe), so no site-level read is owed before the token moves.
    BOOL User32::GetMenuItemInfoT(HMENU hmenu, UINT item, BOOL fByPosition, LPMENUITEMINFO lpmii)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GetMenuItemInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HMENU, UINT, BOOL, LPMENUITEMINFO>(pFunc, hmenu, item, fByPosition, lpmii);
        return bResult;
    }

    //Return policy (U2, table row GetMenuItemRect / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, use the GetLastError function." - the item's RECT is
    //written to the parameter the caller passes, so the BOOL carries no answer and zero is reserved; sdkRetType BOOL so BoolCheck
    //(returncheck.h:94) covers it. AUDIT: the table counts 2 in-tree callers and the site-level reads are menu.cpp:143
    //"return User32::GetMenuItemRect(window ? ... : ... ,MENU_HANDLE, item, reinterpret_cast<LPRECT>(&rcItem)) ? true : false;"
    //and windowcontroller.cpp:486 "return User32::GetMenuItemRect(WINDOW_HANDLE, (HMENU)(menu->GetHandle().Use()), item, reinterpret_cast<LPRECT>(&rc)) ? true : false;" - both sites already collapse the BOOL, so a fired check adds only the diagnostic and each site's false branch is unchanged.
    BOOL User32::GetMenuItemRect(HWND hWnd, HMENU hMenu, UINT uItem, ::LPRECT lprcItem)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetMenuItemRect);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, HMENU, UINT, ::LPRECT>(pFunc, hWnd, hMenu, uItem, lprcItem);
        return bResult;
    }

    //Return policy (U2, table row GetMenuState / menu.cpp, 12 Nov 2026): the row reads checked with checkClass
    //TCheckFailureValue< UINT, ( UINT )-1 >, so the call carries the token - docQuote "Type: UINT If the specified item does not
    //exist, the return value is -1. If the menu item opens a submenu, the low-order byte of the return value contains the menu
    //flags associated with the item, and the high-order byte contains the number of items in the submenu opened by the item.
    //Otherwise, the return value is a mask (Bitwise OR) of the menu flags." with the enumerated flags MF_CHECKED 0x00000008L ,
    //MF_DISABLED 0x00000002L , MF_GRAYED 0x00000001L , MF_HILITE 0x00000080L - an enabled, unhighlighted item answers a mask with
    //no flag bits set, i.e. zero, a legal answer, so keying CheckNonZero< UINT > would report it as a failure while -1 is the
    //documented missing-item value; TCheckFailureValue< UINT, ( UINT )-1 > (returncheck.h:213) passes at every mask and submenu-packed
    //value. AUDIT: the table counts 1 in-tree caller and the site-level read is menu.cpp:148 "return User32::GetMenuState(MENU_HANDLE, id, flags);" - the site forwards the mask as its own answer, so a fired check adds only the continuable diagnostic and every mask passes through unchanged.
    UINT User32::GetMenuState(HMENU hMenu, UINT uId, UINT uFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetMenuState);
        CheckReturn< UINT, TCheckFailureValue< UINT, ( UINT )-1 > >::TType uResult = Library::Call<UINT, HMENU, UINT, UINT>(pFunc, hMenu, uId, uFlags);
        return uResult;
    }

    //Return policy (U2, table row GetMenuString / menu.cpp, 12 Nov 2026): the row reads checked with checkClass CheckNonZero< int >, so
    //the call carries the token - docQuote "Type: int If the function succeeds, the return value specifies the number of characters copied to
    //the buffer, not including the terminating null character. If the function fails, the return value is zero. If the specified item is not of
    //type MIIM_STRING or MFT_STRING , then the return value is zero." - the string lands in the buffer the caller passes, so the count carries no
    //answer, and zero is reserved twice (a failed call and a wrong item type); sdkRetType NO-PROTOTYPE so the base is the published int and
    //CheckNonZero< int > (returncheck.h:187) covers it. CAVEAT recorded: a zero-length string item would answer 0 legitimately, the same caveat
    //the LoadStringA row carries. The table counts 0 in-tree callers, so the change is change-safe and no site-level audit is owed.
    int User32::GetMenuStringT(HMENU hMenu, UINT uIDItem, LPTSTR lpString, int cchMax, UINT flags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GetMenuString);
        CheckReturn< int, CheckNonZero< int > >::TType iResult = Library::Call<int, HMENU, UINT, LPTSTR, int, UINT>(pFunc, hMenu, uIDItem, lpString, cchMax, flags);
        return iResult;
    }

    HMENU User32::GetSubMenu(HMENU hMenu, int nPos)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetSubMenu);
        return Library::Call<HMENU, HMENU, int>(pFunc, hMenu, nPos);
    }

    HMENU User32::GetSystemMenu(HWND hWnd, BOOL bRevert)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetSystemMenu);
        return Library::Call<HMENU, HWND, BOOL>(pFunc, hWnd, bRevert);
    }

    BOOL User32::HiliteMenuItem(HWND hWnd, HMENU hMenu, UINT uIDHiliteItem, UINT uHilite)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, HiliteMenuItem);
        return Library::Call<BOOL, HWND, HMENU, UINT, UINT>(pFunc, hWnd, hMenu, uIDHiliteItem, uHilite);
    }

    //Return policy (U2, table row InsertMenu / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - the menu, the item text and the flags are
    //parameters, so the BOOL carries no answer and zero is reserved; sdkRetType NO-PROTOTYPE so the base is the published BOOL and BoolCheck
    //(returncheck.h:94) covers it; the page is reached via the suffixed ANSI twin slug nf-winuser-insertmenua (CORRECTED leg).
    //The table counts 0 in-tree callers, so the change is change-safe and no site-level audit is owed.
    BOOL User32::InsertMenuT(HMENU hMenu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, LPCTSTR lpNewItem)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, InsertMenu);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HMENU, UINT, UINT, UINT_PTR, LPCTSTR>(pFunc, hMenu, uPosition, uFlags, uIDNewItem, lpNewItem);
        return bResult;
    }

    //Return policy (U2, table row InsertMenuItem / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, use the GetLastError function." - the MENUITEMINFO the caller
    //supplies is a parameter, so the BOOL carries no answer and zero is reserved; sdkRetType NO-PROTOTYPE so the base is the published
    //BOOL and BoolCheck (returncheck.h:94) covers it; the page is reached via the suffixed ANSI twin slug nf-winuser-insertmenuitema
    //(CORRECTED leg). The table counts 0 in-tree callers, so the change is change-safe and no site-level audit is owed.
    BOOL User32::InsertMenuItemT(HMENU hmenu, UINT item, BOOL fByPosition, LPCMENUITEMINFO lpmi)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, InsertMenuItem);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HMENU, UINT, BOOL, LPCMENUITEMINFO>(pFunc, hmenu, item, fByPosition, lpmi);
        return bResult;
    }

    BOOL User32::IsMenu(HMENU hMenu)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsMenu);
        return Library::Call<BOOL, HMENU>(pFunc, hMenu);
    }

    //Return policy (U2, table row LoadMenu / menu.cpp, 12 Nov 2026): the row reads checked with checkClass CheckNonZeroHandle< HMENU >, so
    //the call carries the token - docQuote "Type: HMENU If the function succeeds, the return value is a handle to the menu resource. If the
    //function fails, the return value is NULL . To get extended error information, call GetLastError ." - the page names both a failure
    //condition and the GetLastError route, so NULL is reserved rather than an answer about the caller's parameters, and the handle is what
    //the caller hands to the menu-drawing calls next; sdkRetType NO-PROTOTYPE so the class base is the published HMENU and
    //CheckNonZeroHandle< HMENU > (returncheck.h:137, template< typename THandle >) covers it; deliberately unlike the GetSubMenu /
    //GetSystemMenu rows below, whose pages name no failure condition. The table counts 0 in-tree callers, so the change is
    //change-safe and no site-level audit is owed; the page is reached via the suffixed ANSI twin slug nf-winuser-loadmenua (CORRECTED leg).
    HMENU User32::LoadMenuT(HINSTANCE hInstance, LPCTSTR lpMenuName)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, LoadMenu);
        CheckReturn< HMENU, CheckNonZeroHandle< HMENU > >::TType hResult = Library::Call<HMENU, HINSTANCE, LPCTSTR>(pFunc, hInstance, lpMenuName);
        return hResult;
    }

    //Return policy (U2, table row LoadMenuIndirect / menu.cpp, 12 Nov 2026): the row reads checked with checkClass CheckNonZeroHandle< HMENU >, so
    //the call carries the token - docQuote "Type: HMENU If the function succeeds, the return value is a handle to the menu. If the function
    //fails, the return value is NULL . To get extended error information, call GetLastError ." - the LoadMenu row's reading with the menu built
    //from the indirect string, so NULL is reserved and the handle is the answer the caller uses next; sdkRetType NO-PROTOTYPE so the class base
    //is the published HMENU and CheckNonZeroHandle< HMENU > (returncheck.h:137) covers it; the page is reached via the suffixed ANSI twin
    //slug nf-winuser-loadmenuindirecta (CORRECTED leg). The table counts 0 in-tree callers, so the change is change-safe and no site-level
    //audit is owed.
    HMENU User32::LoadMenuIndirectT(const MENUTEMPLATE* lpMenuTemplate)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, LoadMenuIndirect);
        CheckReturn< HMENU, CheckNonZeroHandle< HMENU > >::TType hResult = Library::Call<HMENU, const MENUTEMPLATE*>(pFunc, lpMenuTemplate);
        return hResult;
    }

    //Return policy (U2, table row MenuItemFromPoint / menu.cpp, 12 Nov 2026): the row reads checked with checkClass TCheckFailureValue< int, -1 >, so
    //the call carries the token - docQuote "Type: int Returns the zero-based position of the menu item at the specified location or -1 if no
    //menu item is at the specified location." - the position is zero-based, so the item at the top of the menu answers 0 legitimately and keying
    //CheckNonZero< int > would report it as a failure; only -1 is reserved, so TCheckFailureValue< int, -1 > (returncheck.h:213) passes at every
    //position. AUDIT: the table counts 2 in-tree callers and the site-level reads are windowcontroller.cpp:499 "return User32::MenuItemFromPoint(WINDOW_HANDLE,
    //(HMENU)(menu ? menu->GetHandle().Use() : nullptr), *(reinterpret_cast<POINT*>(&point)));" inside WindowController::GetMenuItemFromPoint and
    //windowcontroller.cpp:509 "return User32::MenuItemFromPoint(WINDOW_HANDLE, (HMENU)(menu->GetHandle().Use()), *(reinterpret_cast<POINT*>(&screenPoint)));"
    //inside WindowController::MenuItemFromPoint - both wrappers return int and forward the position as their own answer, so a fired check adds only
    //the continuable diagnostic and the -1 no-item sentinel reaches the callers unchanged.
    int User32::MenuItemFromPoint(HWND hWnd, HMENU hMenu, POINT ptScreen)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, MenuItemFromPoint);
        CheckReturn< int, TCheckFailureValue< int, -1 > >::TType iResult = Library::Call<int, HWND, HMENU, POINT>(pFunc, hWnd, hMenu, ptScreen);
        return iResult;
    }

    //Return policy (U2, table row ModifyMenu / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - the menu, the item text and the type are
    //parameters, so the BOOL carries no answer and zero is reserved; sdkRetType NO-PROTOTYPE so the base is the published BOOL and BoolCheck
    //(returncheck.h:94) covers it; the page is reached via the suffixed ANSI twin slug nf-winuser-modifymenua (CORRECTED leg).
    //The table counts 0 in-tree callers, so the change is change-safe and no site-level audit is owed.
    BOOL User32::ModifyMenuT(HMENU hMnu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, LPCTSTR lpNewItem)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, ModifyMenu);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HMENU, UINT, UINT, UINT_PTR, LPCTSTR>(pFunc, hMnu, uPosition, uFlags, uIDNewItem, lpNewItem);
        return bResult;
    }

    //Return policy (U2, table row RemoveMenu / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - the DeleteMenu row's wording word for word,
    //and the menu and item id are parameters, so the BOOL carries no answer and zero is reserved; sdkRetType NO-PROTOTYPE so the base is
    //the published BOOL and BoolCheck (returncheck.h:94) covers it. AUDIT: the table counts 1 in-tree caller and the site-level read is
    //i_m/menu.cpp:188 "return User32::RemoveMenu(MENU_HANDLE, position, flags) ? true : false;" inside Menu::Remove - the site already
    //collapses the BOOL, so a fired check adds only the diagnostic and the site's false branch is unchanged.
    BOOL User32::RemoveMenu(HMENU hMenu, UINT uPosition, UINT uFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, RemoveMenu);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HMENU, UINT, UINT>(pFunc, hMenu, uPosition, uFlags);
        return bResult;
    }

    //Return policy (U2, table row SetMenu / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - the window and the menu handle are
    //parameters, so the BOOL carries no answer and zero is reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it.
    //AUDIT: the table counts 1 in-tree caller and the site-level read is windowcontroller.cpp:514 "return User32::SetMenu(WINDOW_HANDLE,
    //(HMENU)(menu->GetHandle().Use())) ? true : false;" inside WindowController::SetMenu - the site already collapses the BOOL, so a fired
    //check adds only the diagnostic and the site's false branch is unchanged.
    BOOL User32::SetMenu(HWND hWnd, HMENU hMenu)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetMenu);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HWND, HMENU>(pFunc, hWnd, hMenu);
        return bResult;
    }

    //Return policy (U2, table row SetMenuDefaultItem / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, use the GetLastError function." - the default item's identifier is a
    //parameter, so the BOOL carries no answer and zero is reserved; deliberately unlike the GetMenuDefaultItem row above, whose return is the
    //identifier answer with -1 reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. AUDIT: the table counts 1 in-tree caller
    //and the site-level read is i_m/menu.cpp:193 "return User32::SetMenuDefaultItem(MENU_HANDLE, item, byPos) ? true : false;" inside
    //Menu::SetDefaultItem - the site already collapses the BOOL, so a fired check adds only the diagnostic and the site's false branch is unchanged.
    BOOL User32::SetMenuDefaultItem(HMENU hMenu, UINT uItem, UINT fByPos)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetMenuDefaultItem);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HMENU, UINT, UINT>(pFunc, hMenu, uItem, fByPos);
        return bResult;
    }

    //Return policy (U2, table row SetMenuInfo / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - the MENUINFO the caller supplies is a
    //parameter, so the BOOL carries no answer and zero is reserved; deliberately unlike the GetMenuInfo row above only in direction -
    //both take BoolCheck; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. AUDIT: the table counts 1 in-tree caller and the
    //site-level read is i_m/menu.cpp:198 "return User32::SetMenuInfo(MENU_HANDLE, reinterpret_cast<LPCMENUINFO>(&info)) ? true : false;"
    //inside Menu::SetInfo - the site already collapses the BOOL, so a fired check adds only the diagnostic and the site's false branch is unchanged.
    BOOL User32::SetMenuInfo(HMENU hmenu, ::LPCMENUINFO lpcmi)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetMenuInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HMENU, ::LPCMENUINFO>(pFunc, hmenu, lpcmi);
        return bResult;
    }

    //Return policy (U2, table row SetMenuItemBitmaps / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, call GetLastError ." - the check/uncheck bitmaps are parameters,
    //so the BOOL carries no answer and zero is reserved; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it. AUDIT: the table counts
    //1 in-tree caller and the site-level read is i_m/menu.cpp:208 "return User32::SetMenuItemBitmaps(MENU_HANDLE, position, flags, (HBITMAP)(nullptr), (HBITMAP)(nullptr)) ? true : false;"
    //inside Menu::SetItemBitmaps - the site already collapses the BOOL, so a fired check adds only the diagnostic and the site's false branch is
    //unchanged; the site's own nullptr bitmaps are left untouched by this change.
    BOOL User32::SetMenuItemBitmaps(HMENU hMenu, UINT uPosition, UINT uFlags, HBITMAP hBitmapUnchecked, HBITMAP hBitmapChecked)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetMenuItemBitmaps);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HMENU, UINT, UINT, HBITMAP, HBITMAP>(pFunc, hMenu, uPosition, uFlags, hBitmapUnchecked, hBitmapChecked);
        return bResult;
    }

    //Return policy (U2, table row SetMenuItemInfo / menu.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "Type: BOOL If the function succeeds, the return value is nonzero. If the function
    //fails, the return value is zero. To get extended error information, use the GetLastError function." - the MENUITEMINFO the caller
    //supplies is a parameter, so the BOOL carries no answer and zero is reserved; sdkRetType NO-PROTOTYPE so the base is the published
    //BOOL and BoolCheck (returncheck.h:94) covers it; the page is reached via the suffixed ANSI twin slug nf-winuser-setmenuiteminfoa
    //(CORRECTED leg). The table counts 0 in-tree callers, so the change is change-safe and no site-level audit is owed.
    BOOL User32::SetMenuItemInfoT(HMENU hmenu, UINT item, BOOL fByPositon, LPMENUITEMINFO lpmii)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, SetMenuItemInfo);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call<BOOL, HMENU, UINT, BOOL, LPMENUITEMINFO>(pFunc, hmenu, item, fByPositon, lpmii);
        return bResult;
    }

    BOOL User32::TrackPopupMenu(HMENU hMenu, UINT uFlags, int x, int y, int nReserved, HWND hWnd, const RECT* prcRect)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, TrackPopupMenu);
        return Library::Call<BOOL, HMENU, UINT, int, int, int, HWND, const RECT*>(pFunc, hMenu, uFlags, x, y, nReserved, hWnd, prcRect);
    }

    BOOL User32::TrackPopupMenuEx(HMENU hMenu, UINT uFlags, int x, int y, HWND hwnd, ::LPTPMPARAMS lptpm)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, TrackPopupMenuEx);
        return Library::Call<BOOL, HMENU, UINT, int, int, HWND, ::LPTPMPARAMS>(pFunc, hMenu, uFlags, x, y, hwnd, lptpm);
    }

}}}//qor::win::api
