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

    //=== api_layer conventions ========================================= Reviewed 5 Nov 2026
    //   (session 35: correctness / consistency / safety / UB / conventions; 5 wrappers)
    //
    //Five wrappers - the whole "user32.h:594 //Window Properties functions" section: declared :595-:599, all static, all
    //defined here through the User32:: qualifier and in header order (:79 EnumPropsT, :86 EnumPropsExT, :93 GetPropT,
    //:100 RemovePropT, :107 SetPropT), so 5/5 in-section declarations pair with a definition: 0 U4 (declared, never
    //defined), 0 U11 (definition unqualified), 0 U13 (defined, declared non-static), 0 U17 (all 5 members sit over an
    //A/W family and all 5 carry the folder's T suffix). 5/5 members carry qor_pp_fcontext (:81, :88, :95, :102, :109).
    //
    //Return types are the SDK's on 5/5 (U18 clean): "EnumPropsA/W" int at winuser.h:8963, :8969 against int; "EnumPropsExA/W"
    //int at :8943, :8950 against int; "GetPropA/W" HANDLE at :8907, :8913 against HANDLE; "RemovePropA/W" HANDLE at :8925,
    //:8931 against HANDLE; "SetPropA/W" BOOL at :8887, :8894 against BOOL (tmp/_tmp_s35_pre.ps1 section 1 prints each
    //declaration, definition and SDK prototype side by side: 0 return-type divergences, parameter names match the SDK).
    //The three LPCTSTR parameters (:93, :100, :107) against the W prototypes' LPCWSTR are the layer's TCHAR-to-W mapping,
    //i.e. U26's convention, not a defect - and it is exact under this build, not a reinterpret: the probe
    //(tmp/_tmp_s35_cbprobe.cpp, built and run by tmp/_tmp_s35_cbprobe.bat) measures STRICT_defined=1, UNICODE_defined=1,
    //qor_pp_unicode=1 and is_same<LPCTSTR,LPCWSTR>=1.
    //
    //The two callback parameters are checked, not untyped, under this build: is_same<PROPENUMPROC,PROPENUMPROCW>=1 and
    //is_same<PROPENUMPROCEX,PROPENUMPROCEXW>=1 (winuser.h:188, :189 aliasing the STRICT typedefs at :123, :126), so the W
    //entry points receive the callback shape they expect and is_convertible<consumer-callback,PROPENUMPROC>=1 is not a
    //silent widening - the consumer's windowcontroller.cpp:684 "int __stdcall (HWND, LPCTSTR, HANDLE)" IS
    //"BOOL (CALLBACK*)(HWND, LPCWSTR, HANDLE)" because BOOL is int and LPCTSTR is LPCWSTR here. Audit note for the folder:
    //under !STRICT those two names become FARPROC (winuser.h:169, :172) and both parameters stop being type-checked;
    //measured here as not the case (is_same<PROPENUMPROC,FARPROC>=0).
    //
    //Binding (D13): 5 qor_pp_useswinapiAW (:82, :89, :96, :103, :110) over stems EnumProps, EnumPropsEx, GetProp,
    //RemoveProp, SetProp; 5 explicit Library::Call (:83, :90, :97, :104, :111), 0 voidCall. A fresh "dumpbin /exports" of
    //C:\Windows\System32\user32.dll gives each stem "<stem>A" and "<stem>W" and NO bare export (the session-22 x86 capture
    //agrees), so the AW spelling is the only one that could resolve at all and P1/P2/P6 cannot arise here. cl /EP over the
    //TU (tmp/_tmp_win_pp_s35.bat read by tmp/_tmp_s35_ppclass.ps1): 5 binding lines, 5 distinct literals EnumPropsW,
    //EnumPropsExW, GetPropW, RemovePropW, SetPropW, module literal L"user32" 5/5, 0 absent from the export table, 0 ...WW,
    //0 ...T. tmp/_tmp_wrong_stem_scan.ps1 (the wrong-stem class) flags 0 sites in this TU.
    //
    //U1/U2: 0 Win32ErrorHandler and 0 CheckReturn/BoolCheck in the TU, and 0 "_Check_return_" over the 10 A/W prototypes
    //in winuser.h:8884-:8976, so a discarded failure is silent on both sides at once. No check would be sound anyway:
    //GetPropT and RemovePropT return HANDLE and NULL is the SDK's ordinary "no such property" answer, which
    //Library::Call also returns for a failed call (U7), so the layer cannot tell "absent" from "call failed" - and
    //EnumPropsT/EnumPropsExT return an int in which 0 is legal. SetPropT's BOOL is answer-shaped but the consumer collapses
    //it to bool (windowcontroller.cpp:712). Policy settled 29 Oct 2026: recorded, nothing added.
    //
    //U20: the HANDLE this TU hands back is not a kernel handle the caller owns - consumers store pointers in the property
    //(windowfactory.cpp:51, :60 and dialog.cpp:47, :57, :64 reinterpret_cast to Window*/Dialog*) and
    //windowcontroller.cpp:702, :707 pass it on as "void*" with no ownership metadata, so the TU feeds U20 through the
    //pointer-as-handle channel and no destroy path hangs off it.
    //U19: 9 live in-tree call sites, all of them through this layer - windowcontroller.cpp:695, :702, :707, :712,
    //windowfactory.cpp:51, :60, dialog.cpp:47, :57, :64 - and 0 live SDK bypass: the four GetPropW/SetPropW/RemoveProp sites
    //in gl/windowimpl.cpp (:534, :1427, :1597, :2131) sit inside block comments opened at :530, :1303, :2092, which the
    //comment-stripping scan (tmp/_tmp_s35_pre.ps1 section 5) found and the comment markers confirm.
    //
    //U16: all five pairs sit at guard depth 3 - "#ifndef _WINUSER_", "#ifndef NOUSER",
    //WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) - over winuser.h:8884-:8976, none with a WINVER or _WIN32_WINNT
    //floor, so no availability floor to record here. U25: 0 CONST and 0 lowercase const in this TU and in its 5 header
    //declarations - clean. T7: the #include "../returncheck.h" at (then) :11 was unused here (no CheckReturn, BoolCheck
    //or Win32ErrorHandler) - one of the folder's 35 dead includes; removed under T7 on 5 Nov 2026, the folder-wide
    //not a per-TU pass. U14: :97 writes Library::Call< HANDLE, HWND, LPCTSTR>( with no space before the "(" - U14 is
    //frozen (D15) and the user decided on 5 Nov 2026 to leave it; left as-is, no tally booked.
    //
    //Hand-off correction to session 34's pre-flags: SetLayeredWindowAttributes is NOT declared-without-definition - it is
    //declared at user32.h:196 (another section) and defined at window.cpp:565 with bare qor_pp_useswinapi (:568), the
    //D13 spelling for a name with ONE export on both arches and no A/W family (prototype winuser.h:4633, DESKTOP family,
    //no version floor). Not a U4 candidate, not a session-35 item: window.cpp's (session 33).
    //
    //Addendum, 12 Nov 2026 (U2 Small TU batch S4, item windowproperties sites): the U1/U2 paragraph above is the review's
    //record and stands superseded for one of the five rows - the endpoint table now reads SetProp checked with checkClass
    //BoolCheck and only EnumProps, EnumPropsEx, GetProp and RemoveProp unchecked, so this TU carries 1 CheckReturn site
    //and regained its one ../returncheck.h include (0 -> 1; the include this TU removed under T7 on 5 Nov 2026 is back
    //because the check that made it live is here now). The decision is the table's, not re-derived here; each row's
    //verbatim docQuote and its written reason sits at the site below, and the four unchecked rows keep 0 CheckReturn.

    //Return policy (U2, table row EnumProps / windowproperties.cpp, 12 Nov 2026): the row reads UNCHECKED with no check
    //class, and the table's verbatim docQuote is "The return value specifies the last value returned by the callback
    //function. It is -1 if the function did not find a property for enumeration." - the value is whatever the caller's own
    //lpEnumFunc last returned, so every int a callback hands back is a legal answer and -1 is a documented outcome, not a
    //reserved failure; the session-27 soundness test (a checked-failure site is only sound where the SDK's failure value
    //is not also a legal success value) fails here - a callback that returns -1 for its own reason would be reported as a
    //call failure. 1 in-tree caller re-read before this verdict was accepted: ui/gui/controllers/windowcontroller.cpp:695
    //stores the int in "int result", then returns it as its own int at :699 with no test, so nothing at the call site
    //depends on a failure spelling. This site keeps 0 CheckReturn and hands the value over as written - AW binding, int.
    int User32::EnumPropsT(HWND hWnd, PROPENUMPROC lpEnumFunc)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, EnumProps);
        return Library::Call< int, HWND, PROPENUMPROC >(pFunc, hWnd, lpEnumFunc);
    }

    //Return policy (U2, table row EnumPropsEx / windowproperties.cpp, 12 Nov 2026): the row reads UNCHECKED with no check
    //class, and the table's verbatim docQuote is "The return value specifies the last value returned by the callback
    //function. It is -1 if the function did not find a property for enumeration." - same callback-shaped int as the
    //sibling row: the value is the last thing the caller's lpEnumFunc returned, so the SDK reserves nothing and the
    //session-27 soundness test fails at -1, which is a documented outcome rather than a failure. 0 in-tree callers for
    //this stem (the table's callers column and the U19 scan agree), so no caller reading depends on a failure spelling.
    //This site keeps 0 CheckReturn and hands the value over as written - AW binding, int, LPARAM lParam passed through.
    int User32::EnumPropsExT(HWND hWnd, PROPENUMPROCEX lpEnumFunc, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, EnumPropsEx);
        return Library::Call< int, HWND, PROPENUMPROCEX, LPARAM >(pFunc, hWnd, lpEnumFunc, lParam);
    }

    //Return policy (U2, table row GetProp / windowproperties.cpp, 12 Nov 2026): the row reads UNCHECKED with no check
    //class, and the table's verbatim docQuote is "If the property list contains the string, the return value is the
    //associated data handle. Otherwise, the return value is NULL" - the handle is the answer this stem produces, so NULL
    //is the documented answer for "no such property" and nothing is reserved; the session-27 soundness test fails at that
    //value, and a CheckReturnHandle here would report a caller's legitimate absent-property read as a failure (that is
    //also the U7 collision the review above records: Library::Call returns NULL for a failed call too, so the layer
    //cannot separate the two without a check that would be unsound). 4 in-tree call sites for the T spelling, all re-read
    //before this verdict was accepted: ui/gui/controllers/windowcontroller.cpp:702 returns the handle untouched, and
    //ui/gui/factories/windowfactory.cpp:51 and ui/gui/windows/a_h/dialog.cpp:57 / :64 test it against nullptr on their
    //own - exactly the "absent means create it" reading the page describes. This site keeps 0 CheckReturn and hands the
    //handle over as written - AW binding, HANDLE.
    HANDLE User32::GetPropT(HWND hWnd, LPCTSTR lpString)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GetProp);
        return Library::Call< HANDLE, HWND, LPCTSTR>(pFunc, hWnd, lpString);
    }

    //Return policy (U2, table row RemoveProp / windowproperties.cpp, 12 Nov 2026): the row reads UNCHECKED with no check
    //class, and the table's verbatim docQuote is "The return value identifies the specified data. If the data cannot be
    //found in the specified property list, the return value is NULL" - answer-shaped handle again, same shape as the
    //GetProp row: NULL is a documented outcome the caller may legitimately hold, nothing is reserved, and the session-27
    //soundness test fails at that value, so a check here would report a "nothing of that name to remove" read as a
    //failure. 1 in-tree call site for the T spelling, re-read before this verdict was accepted: ui/gui/controllers/
    //windowcontroller.cpp:707 returns the handle straight through as its own void* with no test on it, which is exactly
    //the "hand back whatever the list gave" reading. This site keeps 0 CheckReturn and hands the handle over as written.
    HANDLE User32::RemovePropT(HWND hWnd, LPCTSTR lpString)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, RemoveProp);
        return Library::Call< HANDLE, HWND, LPCTSTR >(pFunc, hWnd, lpString);
    }

    //Return policy (U2, table row SetProp / windowproperties.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "If the data handle and string are added to the property list, the
    //return value is nonzero. If the function fails, the return value is zero. To get extended error information, call
    //GetLastError" - the property this stem adds is the caller's own hData, which leaves through the property list rather
    //than through the return, so the returned value is pure status and zero is reserved for failure; the session-27
    //soundness test (a checked-failure site is only sound where the SDK's failure value is not also a legal success
    //value) passes at every value, and BOOL + zero-means-failure takes the BoolCheck the library already has (returncheck.h
    //:94, Test at :99 is value ? true : false) - no new check class. The row is an AW binding whose prototype the census
    //could not resolve (NO-PROTOTYPE), so the class base is the published BOOL this wrapper declares, not an SDK spelling.
    //The table's callers column reads 0 for the exact stem, while the U19 scan above names the T spelling at
    //ui/gui/controllers/windowcontroller.cpp:712, ui/gui/factories/windowfactory.cpp:60 and ui/gui/windows/a_h/
    //dialog.cpp:47 - all three re-read before this token moved: :712 folds this BOOL into its own bool with "? true :
    //false", so the new throw replaces that caller's false path with a continuable issue the Win32ErrorHandler above
    //reports, and :60 / :47 discard the value entirely, so they now get the issue where they used to continue silently
    //over a FALSE. The wrapper still forwards hWnd/lpString/hData exactly as those callers wrote them, and G6 keeps the
    //check live on every call.
    BOOL User32::SetPropT(HWND hWnd, LPCTSTR lpString, HANDLE hData)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, SetProp);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWND, LPCTSTR, HANDLE >(pFunc, hWnd, lpString, hData);
        return bResult;
    }

}}}//qor::win::api
