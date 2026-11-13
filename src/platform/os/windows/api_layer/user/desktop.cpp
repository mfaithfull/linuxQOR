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

    //The "Window Station and Desktop functions" section (user32.h:399-417): 18 declarations, 18 definitions here,
    //in the header's order, and each definition matches its declaration parameter-for-parameter
    //(tmp/_tmp_desktop_sigcmp.ps1). Nine of the entry points have an SDK A/W family (winuser.h:1396-1447
    //CreateDesktop/CreateDesktopEx, :1461-1478 OpenDesktop, :1492-1507 EnumDesktops, :1583-1600 CreateWindowStation,
    //:1605-1620 OpenWindowStation, :1625-1638 EnumWindowStations, :1711-1730 GetUserObjectInformation,
    //:1735-1752 SetUserObjectInformation), so those nine wrappers spell the binding qor_pp_useswinapiAW and the other
    //nine (CloseDesktop, CloseWindowStation, GetProcessWindowStation, GetThreadDesktop, OpenInputDesktop,
    //SetProcessWindowStation, SetThreadDesktop, SwitchDesktop, EnumDesktopWindows - all declared directly, no A/W
    //family, no stem -> stemW macro) use the bare qor_pp_useswinapi. cl /EP on this TU resolves "OpenDesktopW",
    //"CloseDesktop", ... and all 18 names are in the export table and resolve live through the library's own
    //GetModuleHandleW/GetProcAddress step (tmp/_tmp_desktop_probe.cpp, which also shows GetProcessWindowStation and
    //GetThreadDesktop returning live non-null handles).
    //Handle contracts: OpenDesktopT/CreateDesktopT/CreateDesktopExT/OpenInputDesktop return an HDESK and
    //CreateWindowStationT/OpenWindowStationT an HWINSTA. Both families are released by CloseDesktop/CloseWindowStation
    //in this same section, NOT by Kernel32::CloseHandle; the handles are returned raw and nothing here is RAII-wrapped
    //(open item U2), so a caller that drops one leaks a desktop or window station object.
    //Thread contracts: GetThreadDesktop/SetThreadDesktop/SwitchDesktop/OpenInputDesktop act on the calling thread or the
    //process window station. SetThreadDesktop fails if the calling thread has already created a window on that
    //desktop or installed a hook there, and SwitchDesktop is per-process, not per-thread.
    //Callbacks: EnumDesktopsT/EnumWindowStationsT take the SDK's **W** spellings DESKTOPENUMPROCW/WINSTAENUMPROCW (winuser.h:185
    //and :184, which alias NAMEENUMPROCW at :180 = BOOL(CALLBACK*)(LPWSTR,LPARAM)), matching the prototype parameters the
    //entry points actually take at winuser.h:1480 and :1610 - NOT the generic DESKTOPENUMPROC/WINSTAENUMPROC, which reach
    //the W shape only through the alias at winuser.h:202/:201 and whose !UNICODE arm at :207/:206 swaps in the A shape, so
    //the generic spelling hides which callback a caller has to supply while this section always binds "EnumDesktopsW"/
    //EnumWindowStationsW. Task T2, decision D25: the W spelling went into the declaration (user32.h:410, :413), the
    //definition (:86, :107) and both Library::Call argument lists (:90, :111). It moves no type and no mangled name -
    //tmp/_tmp_t2_probe_id.cpp compiles at exit 0 with is_same<DESKTOPENUMPROC,DESKTOPENUMPROCW>=1, is_same<WINSTAENUMPROC,
    //WINSTAENUMPROCW>=1 and both equal to BOOL(__stdcall*)(LPWSTR,LPARAM), so a caller hands the same function pointer
    //either way - and it is not an untyped parameter: tmp/_tmp_t2_probe_neg.cpp exits 2, an A-shaped BOOL(__stdcall*)
    //(LPSTR,LPARAM) is still rejected. Caveat already recorded in windowproperties.cpp:33-39: under !STRICT winuser.h:195
    //and :194 make these four names FARPROC and the parameter stops being type-checked; this build defines STRICT and
    //qor_pp_unicode 1, so that arm is not the case here. The :220-227 citation this block carried before the edit was
    //stale - the typedefs measure at :179-:207 in this WinUser.h.
    //Section coverage: within this section every SDK entry point in the winuser.h NODESKTOP / NOWINDOWSTATION regions
    //(:1391-1448, :1456-1546, :1548-1661) is wrapped. The two user-object security calls the SDK declares next to
    //GetUserObjectInformation/SetUserObjectInformation - SetUserObjectSecurity (:1671-1674) and GetUserObjectSecurity
    //(:1679-1684, exports 375 and 1DB) - are declared in user32.h but in a different section, "//Authorization
    //functions" :657-659, where they are the section's only two members and are declared WITHOUT static; neither has a
    //definition in any TU in the folder, so a caller of either gets LNK2019 (open item U4, and the non-static shape is
    //open item U18). UserHandleGrantAccess (:14925-14928, export 3D8), the third user-object call, is declared at
    //user32.h:259 in the "Process and Thread functions" section and defined in processthread.cpp:41, not here.
    //No wrapper in this TU has a Win32ErrorHandler _; (0/18, G5) - settled library policy.

    //Return policy (U2, table row OpenDesktop / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HDESK >, and the table's verbatim docQuote is "If the function succeeds, the return value is a
    //handle to the opened desktop. When you are finished using the handle, call the CloseDesktop function to close it.
    //If the function fails, the return value is NULL . To get extended error information, call GetLastError ." - NULL is
    //reserved and the success answer is a handle, which is never NULL, so the session-27 soundness test (a checked-failure
    //site is only sound where the SDK's failure value is not also a legal success value) passes at every value; the AW
    //row has NO-PROTOTYPE sdkRetType so the class base is the published HDESK, and CheckNonZeroHandle (returncheck.h:137,
    //Test at :144 is (value == 0x00000000) ? false : true) is already in the library - no new check class. 0 in-tree
    //callers for the exact stem (D14 published surface), so nothing built today reaches this site; G6 keeps the check
    //live on every call, so a failed open now raises a continuable issue the Win32ErrorHandler above reports rather than
    //travelling as a silent NULL handle.
    HDESK User32::OpenDesktopT(LPCTSTR lpszDesktop, DWORD dwFlags, BOOL fInherit, ACCESS_MASK dwDesiredAccess)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, OpenDesktop);
        CheckReturn< HDESK, CheckNonZeroHandle< HDESK > >::TType hResult = Library::Call< HDESK, LPCTSTR, DWORD, BOOL, ACCESS_MASK >(pFunc, lpszDesktop, dwFlags, fInherit, dwDesiredAccess);
        return hResult;
    }

    //Return policy (U2, table row CloseDesktop / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, call GetLastError ." - zero is reserved
    //and the BOOL carries no answer of its own (the desktop being closed is the hDesktop parameter), so the session-27
    //soundness test passes at every value; sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94, Test at
    //:101 is value ? true : false) is the class the library already has - no new check class. 0 in-tree callers for the
    //exact stem (D14 published surface), so nothing built today reaches this site; G6 keeps the check live on every call,
    //so a failed close now raises a continuable issue rather than travelling as a silent success.
    BOOL User32::CloseDesktop(HDESK hDesktop)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CloseDesktop);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HDESK >(pFunc, hDesktop);
        return bResult;
    }

    //Return policy (U2, table row CreateDesktop / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HDESK >, and the table's verbatim docQuote is "If the function succeeds, the return value is a
    //handle to the newly created desktop. If the specified desktop already exists, the function succeeds and returns a
    //handle to the existing desktop. When you are finished using the handle, call the CloseDesktop function to close it.
    //If the function fails, the return value is NULL . To get extended error information, call GetLastError ." - both
    //success branches answer with a handle and NULL is reserved for failure, so no legal answer collides with the tested
    //value; AW row with NO-PROTOTYPE sdkRetType so the class base is the published HDESK, and CheckNonZeroHandle
    //(returncheck.h:137, Test at :144 is (value == 0x00000000) ? false : true) is already in the library. 0 in-tree
    //callers for the exact stem (D14 published surface); G6 keeps the check live on every call.
    HDESK User32::CreateDesktopT(LPCTSTR lpszDesktop, LPCTSTR lpszDevice, ::LPDEVMODE pDevmode, DWORD dwFlags, ACCESS_MASK dwDesiredAccess, ::LPSECURITY_ATTRIBUTES lpsa)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, CreateDesktop);
        CheckReturn< HDESK, CheckNonZeroHandle< HDESK > >::TType hResult = Library::Call< HDESK, LPCTSTR, LPCTSTR, ::LPDEVMODE, DWORD, ACCESS_MASK, ::LPSECURITY_ATTRIBUTES >(pFunc, lpszDesktop, lpszDevice, pDevmode, dwFlags, dwDesiredAccess, lpsa);
        return hResult;
    }

    //Return policy (U2, table row CreateDesktopEx / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HDESK >, and the table's verbatim docQuote is the CreateDesktop row's wording - "If the function
    //succeeds, the return value is a handle to the newly created desktop. If the specified desktop already exists, the
    //function succeeds and returns a handle to the existing desktop. When you are finished using the handle, call the
    //CloseDesktop function to close it. If the function fails, the return value is NULL. To get extended error
    //information, call GetLastError ." - handle answers on both success branches and NULL reserved, so the session-27
    //soundness test passes at every value; AW row with NO-PROTOTYPE sdkRetType so the class base is the published HDESK,
    //and CheckNonZeroHandle (returncheck.h:137, Test at :144 is (value == 0x00000000) ? false : true) is already in the
    //library. 0 in-tree callers for the exact stem (D14 published surface); G6 keeps the check live on every call.
    HDESK User32::CreateDesktopExT(LPCTSTR lpszDesktop, LPCTSTR lpszDevice, ::LPDEVMODE pDevmode, DWORD dwFlags, ACCESS_MASK dwDesiredAccess, ::LPSECURITY_ATTRIBUTES lpsa, ULONG ulHeapSize, PVOID pvoid)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, CreateDesktopEx);
        CheckReturn< HDESK, CheckNonZeroHandle< HDESK > >::TType hResult = Library::Call< HDESK, LPCTSTR, LPCTSTR, ::LPDEVMODE, DWORD, ACCESS_MASK, ::LPSECURITY_ATTRIBUTES, ULONG, PVOID >(pFunc, lpszDesktop, lpszDevice, pDevmode, dwFlags, dwDesiredAccess, lpsa, ulHeapSize, pvoid);
        return hResult;
    }

    //Return policy (U2, table row EnumDesktops / desktop.cpp, 12 Nov 2026): the row reads unchecked with no checkClass,
    //and the table's verbatim docQuote is "If the function succeeds, it returns the nonzero value returned by the callback
    //function that was pointed to by lpEnumFunc . If the function is unable to perform the enumeration, the return value
    //is zero. Call GetLastError to get extended error information. If the callback function fails, the return value is
    //zero." - the return is the callback's own answer, not a status, and zero comes from two different branches: a real
    //failure ("unable to perform the enumeration") and the caller's legal decision to stop ("If the callback function
    //fails"), so keying a check on zero would report that legal stop as a failure and the session-27 soundness test fails
    //at that value. No token is landed; the callback's value and the SDK's zero travel back exactly as produced. 0 in-tree
    //callers for the exact stem (D14 published surface).
    BOOL User32::EnumDesktopsT(HWINSTA hwinsta, DESKTOPENUMPROCW lpEnumFunc, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, EnumDesktops);
        return Library::Call< BOOL, HWINSTA, DESKTOPENUMPROCW, LPARAM >(pFunc, hwinsta, lpEnumFunc, lParam);
    }

    //Return policy (U2, table row CreateWindowStation / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HWINSTA >, and the table's verbatim docQuote is "If the function succeeds, the return value is a
    //handle to the newly created window station. If the specified window station already exists, the function succeeds
    //and returns a handle to the existing window station. If the function fails, the return value is NULL . To get
    //extended error information, call GetLastError ." - the window-station mirror of the CreateDesktop row: handle answers
    //on both success branches, NULL reserved, so the session-27 soundness test passes at every value; AW row with
    //NO-PROTOTYPE sdkRetType so the class base is the published HWINSTA, and CheckNonZeroHandle (returncheck.h:137, Test
    //at :144 is (value == 0x00000000) ? false : true) is already in the library. 0 in-tree callers for the exact stem
    //(D14 published surface); G6 keeps the check live on every call.
    HWINSTA User32::CreateWindowStationT(LPCTSTR lpwinsta, DWORD dwFlags, ACCESS_MASK dwDesiredAccess, ::LPSECURITY_ATTRIBUTES lpsa)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, CreateWindowStation);
        CheckReturn< HWINSTA, CheckNonZeroHandle< HWINSTA > >::TType hResult = Library::Call< HWINSTA, LPCTSTR, DWORD, ACCESS_MASK, ::LPSECURITY_ATTRIBUTES >(pFunc, lpwinsta, dwFlags, dwDesiredAccess, lpsa);
        return hResult;
    }

    //Return policy (U2, table row CloseWindowStation / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, call GetLastError . Windows Server 2003
    //and Windows XP/2000: This function does not set the last error code on failure." - zero is reserved and the BOOL
    //carries no answer of its own (the window station being closed is the hWinSta parameter), so the session-27 soundness
    //test passes at every value; sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94, Test at :101 is
    //value ? true : false) is the class the library already has. The trailing legacy note only says the last error may be
    //absent on old systems - it does not make zero a legal answer. 0 in-tree callers for the exact stem (D14 published
    //surface); G6 keeps the check live on every call.
    BOOL User32::CloseWindowStation(HWINSTA hWinSta)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CloseWindowStation);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWINSTA >(pFunc, hWinSta);
        return bResult;
    }

    //Return policy (U2, table row EnumWindowStations / desktop.cpp, 12 Nov 2026): the row reads unchecked with no checkClass,
    //and the table's verbatim docQuote is word for word the EnumDesktops row's - "If the function succeeds, it returns the
    //nonzero value returned by the callback function that was pointed to by lpEnumFunc . If the function is unable to
    //perform the enumeration, the return value is zero. Call GetLastError to get extended error information. If the
    //callback function fails, the return value is zero." - so the same callback shape applies: the callback's own value is
    //the answer and zero comes both from a real failure and from the caller's legal decision to stop, where the second
    //branch is a legal outcome, so the session-27 soundness test fails at zero. No token; the value travels as produced.
    //0 in-tree callers for the exact stem (D14 published surface).
    BOOL User32::EnumWindowStationsT(WINSTAENUMPROCW lpEnumFunc, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, EnumWindowStations);
        return Library::Call< BOOL, WINSTAENUMPROCW, LPARAM >(pFunc, lpEnumFunc, lParam);
    }

    //Return policy (U2, table row GetProcessWindowStation / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HWINSTA >, and the table's verbatim docQuote is "If the function succeeds, the return value is a
    //handle to the window station. If the function fails, the return value is NULL. To get extended error information,
    //call GetLastError ." - NULL is reserved and the success answer is a handle, which is never NULL, so the session-27
    //soundness test passes at every value; the row's sdkRetType column is blank because the SDK declares this prototype
    //over separate lines, so the class base is the published HWINSTA, and CheckNonZeroHandle (returncheck.h:137, Test at
    //:144 is (value == 0x00000000) ? false : true) is already in the library. 0 in-tree callers for the exact stem (D14
    //published surface); G6 keeps the check live on every call, so a NULL station handle now raises an issue rather than
    //travelling into the process window-station slot.
    HWINSTA User32::GetProcessWindowStation(void)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetProcessWindowStation);
        CheckReturn< HWINSTA, CheckNonZeroHandle< HWINSTA > >::TType hResult = Library::Call< HWINSTA >(pFunc);
        return hResult;
    }

    //Return policy (U2, table row GetThreadDesktop / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HDESK >, and the table's verbatim docQuote is "If the function succeeds, the return value is a
    //handle to the desktop associated with the specified thread. You do not need to call the CloseDesktop function to
    //close the returned handle. If the function fails, the return value is NULL. To get extended error information, call
    //GetLastError ." - NULL is reserved and the success answer is a handle, so the session-27 soundness test passes at
    //every value; the Remark only says the returned handle needs no CloseDesktop, it does not make NULL a legal answer.
    //The row's sdkRetType column is blank for the same separate-line prototype as the GetProcessWindowStation row, so the
    //class base is the published HDESK and CheckNonZeroHandle (returncheck.h:137) is already in the library. 0 in-tree
    //callers for the exact stem (D14 published surface); G6 keeps the check live on every call.
    HDESK User32::GetThreadDesktop(DWORD dwThreadId)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetThreadDesktop);
        CheckReturn< HDESK, CheckNonZeroHandle< HDESK > >::TType hResult = Library::Call< HDESK, DWORD >(pFunc, dwThreadId);
        return hResult;
    }

    //Return policy (U2, table row GetUserObjectInformation / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, call GetLastError ." - zero is reserved
    //and the BOOL carries no answer of its own (the object's information lands in the pvInfo out-parameter and its
    //required size in lpnLengthNeeded), so the session-27 soundness test passes at every value; the AW row's NO-PROTOTYPE
    //sdkRetType gives the published BOOL as the base and BoolCheck (returncheck.h:94, Test at :101 is value ? true :
    //false) is the class the library already has. The Remark is only the winuser.h encoding-alias note. 0 in-tree callers
    //for the exact stem (D14 published surface); G6 keeps the check live on every call.
    BOOL User32::GetUserObjectInformationT(HANDLE hObj, int nIndex, PVOID pvInfo, DWORD nLength, LPDWORD lpnLengthNeeded)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, GetUserObjectInformation);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, int, PVOID, DWORD, LPDWORD >(pFunc, hObj, nIndex, pvInfo, nLength, lpnLengthNeeded);
        return bResult;
    }

    //Return policy (U2, table row OpenInputDesktop / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HDESK >, and the table's verbatim docQuote is "If the function succeeds, the return value is a
    //handle to the desktop that receives user input. When you are finished using the handle, call the CloseDesktop
    //function to close it. If the function fails, the return value is NULL . To get extended error information, call
    //GetLastError ." - NULL is reserved and the success answer is a handle, which is never NULL, so the session-27
    //soundness test passes at every value; the row's sdkRetType column is blank because the SDK declares this prototype
    //over separate lines (winuser.h:1481-1483), so the class base is the published HDESK, which is the SDK type too.
    //CheckNonZeroHandle (returncheck.h:137, Test at :144 is (value == 0x00000000) ? false : true) is already in the
    //library. 0 in-tree callers for the exact stem (D14 published surface); G6 keeps the check live on every call.
    HDESK User32::OpenInputDesktop(DWORD dwFlags, BOOL fInherit, ACCESS_MASK dwDesiredAccess)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, OpenInputDesktop);
        CheckReturn< HDESK, CheckNonZeroHandle< HDESK > >::TType hResult = Library::Call< HDESK, DWORD, BOOL, ACCESS_MASK >(pFunc, dwFlags, fInherit, dwDesiredAccess);
        return hResult;
    }

    //Return policy (U2, table row OpenWindowStation / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //CheckNonZeroHandle< HWINSTA >, and the table's verbatim docQuote is "If the function succeeds, the return value is
    //the handle to the specified window station. If the function fails, the return value is NULL . To get extended
    //error information, call GetLastError ." - NULL is reserved and the success answer is a handle, which is never NULL,
    //so the session-27 soundness test passes at every value; the Remark "After you are done with the handle, you must
    //call CloseWindowStation to free the handle" is ownership, not a legal zero. The AW row's sdkRetType is NO-PROTOTYPE,
    //so the class base is the published HWINSTA, and CheckNonZeroHandle (returncheck.h:137, Test at :144 is
    //(value == 0x00000000) ? false : true) is already in the library. 0 in-tree callers for the exact stem (D14 published
    //surface); G6 keeps the check live on every call.
    HWINSTA User32::OpenWindowStationT(LPCTSTR lpszWinSta, BOOL fInherit, ACCESS_MASK dwDesiredAccess)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, OpenWindowStation);
        CheckReturn< HWINSTA, CheckNonZeroHandle< HWINSTA > >::TType hResult = Library::Call< HWINSTA, LPCTSTR, BOOL, ACCESS_MASK >(pFunc, lpszWinSta, fInherit, dwDesiredAccess);
        return hResult;
    }

    //Return policy (U2, table row SetProcessWindowStation / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, call GetLastError ." - zero is reserved
    //and the BOOL carries no answer of its own (the window station being installed is the HWINSTA parameter), so the
    //session-27 soundness test passes at every value; sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94,
    //Test at :101 is value ? true : false) is the class the library already has. 0 in-tree callers for the exact stem
    //(D14 published surface); G6 keeps the check live on every call.
    BOOL User32::SetProcessWindowStation(HWINSTA hWinSta)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetProcessWindowStation);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HWINSTA >(pFunc, hWinSta);
        return bResult;
    }

    //Return policy (U2, table row SetThreadDesktop / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, call GetLastError ." - the same two-line
    //status wording the SetProcessWindowStation row carries, so zero is reserved and the BOOL carries no answer of its own
    //; the Remark "The SetThreadDesktop function will fail if the calling thread has any windows or hooks on its current
    //desktop (unless the hDesktop parameter is a handle to the current desktop)" names that case as a failure rather than
    //as a legal zero. sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94) is already in the library. 0
    //in-tree callers for the exact stem (D14 published surface); G6 keeps the check live on every call.
    BOOL User32::SetThreadDesktop(HDESK hDesktop)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetThreadDesktop);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HDESK >(pFunc, hDesktop);
        return bResult;
    }

    //Return policy (U2, table row SetUserObjectInformation / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "If the function succeeds, the return value is nonzero. If the function
    //fails the return value is zero. To get extended error information, call GetLastError ." (this page omits the comma
    //before "the return value is zero") - zero is reserved and the BOOL carries no answer of its own, the object's data
    //being the pvInfo/nLength parameters; the Remark is the same winuser.h encoding-alias note as the GetUserObjectInformation
    //row. The AW row's sdkRetType is NO-PROTOTYPE, so the class base is the published BOOL and BoolCheck (returncheck.h:94)
    //is already in the library. 0 in-tree callers for the exact stem (D14 published surface); G6 keeps the check live on
    //every call.
    BOOL User32::SetUserObjectInformationT(HANDLE hObj, int nIndex, PVOID pvInfo, DWORD nLength)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, SetUserObjectInformation);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, int, PVOID, DWORD >(pFunc, hObj, nIndex, pvInfo, nLength);
        return bResult;
    }

    //Return policy (U2, table row SwitchDesktop / desktop.cpp, 12 Nov 2026): the row reads checked with checkClass
    //BoolCheck, and the table's verbatim docQuote is "If the function succeeds, the return value is nonzero. If the
    //function fails, the return value is zero. To get extended error information, call GetLastError . However, SwitchDesktop
    //only sets the last error for the following cases: When the desktop belongs to an invisible window station When hDesktop
    //is an invalid handle, refers to a destroyed desktop, or belongs to a different session than that of the calling process"
    //- zero is reserved and the BOOL carries no answer of its own (the desktop being switched to is the HDESK parameter);
    //the follow-up clause withholds extended error info on some failures and the Remark "The SwitchDesktop function fails
    //if the desktop belongs to an invisible window station" names zero as a failure, so neither makes zero a legal answer.
    //sdkRetType BOOL so the base is BOOL and BoolCheck (returncheck.h:94) is already in the library. 0 in-tree callers
    //for the exact stem (D14 published surface); G6 keeps the check live on every call.
    BOOL User32::SwitchDesktop(HDESK hDesktop)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SwitchDesktop);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HDESK >(pFunc, hDesktop);
        return bResult;
    }

    //No check landed (U2, table row EnumDesktopWindows / desktop.cpp, 12 Nov 2026): the row reads unchecked with no checkClass
    //and no token, on the callback answer shape rather than a plain status. The table's verbatim docQuote is "If the function
    //fails or is unable to perform the enumeration, the return value is zero. To get extended error information, call
    //GetLastError . You must ensure that the callback function sets SetLastError if it fails. Windows Server 2003 and
    //Windows XP/2000: If there are no windows on the desktop, GetLastError returns ERROR_INVALID_HANDLE ." - "If the
    //function fails or is unable to perform the enumeration" and "You must ensure that the callback function sets
    //SetLastError if it fails" put both the stop and the error in the caller's own callback, and the Remark "The
    //EnumDesktopWindows function repeatedly invokes the lpfn callback function until the last top-level window is
    //enumerated or the callback function returns FALSE" makes a callback that returns FALSE to stop early a legal caller
    //decision that still answers zero; same shape as the EnumDesktops and EnumWindowStations rows in this TU, so keying
    //a check on zero would report that legal stop as a failure and the session-27 soundness test fails at that value.
    //sdkRetType BOOL, so BoolCheck would have been available - it is deliberately not landed. 0 in-tree callers for the
    //exact stem (D14 published surface).
    BOOL User32::EnumDesktopWindows(HDESK hDesktop, ::WNDENUMPROC lpfn, LPARAM lParam)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, EnumDesktopWindows);
        return Library::Call< BOOL, HDESK, ::WNDENUMPROC, LPARAM >(pFunc, hDesktop, lpfn, lParam);
    }

}}}//qor::win::api
