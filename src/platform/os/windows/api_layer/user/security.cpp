// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "../kernel/errorhandler.h"

//=== api_layer conventions ========================================= Added 8 Nov 2026
//   (T1: the two declared-but-undefined members in the "//Authorization functions" section - new TU)
//
//This TU closes task T1 (src/platform/os/windows/api_layer/user/registers/tasks.md): user32.h's "//Authorization
//functions" section declares GetUserObjectSecurity and SetUserObjectSecurity, but no .cpp in the folder defines either,
//so a caller of either fails at link time with LNK2019 rather than at compile time (open item U4 - declared-but-undefined),
//and both declarations are non-static, the folder's U18 shape. The fix lands both halves together: this TU adds the two
//definitions, and user32.h gains `static` on both declarations, matching the 427-of-525 static majority that the desktop
//user-object pair just above (desktop.cpp:118 GetUserObjectInformationT / :153 SetUserObjectInformationT) already stands
//under.
//
//Both entry points are real, single-stem exports of user32.dll, not an A/W family, so both bindings spell the bare
//qor_pp_useswinapi (tmp/_tmp_user32_exports.txt: GetUserObjectSecurity ordinal 1DB hint 1979 RVA 00002460, and
//SetUserObjectSecurity ordinal 375 hint 2384 RVA 0002BBE0 - one row each, no A/W sibling), and cl /EP on this TU resolves
//exactly those two binding literals against module literal "user32".
//
//Signature rule (D18, docs/agent/DECISIONS.md): the published signature IS the winuser.h prototype, parameter names
//included. The SDK declares both directly after SetUserObjectInformation/GetUserObjectInformation - the user-object family
//desktop.cpp already wraps - and each definition here copies that prototype exactly, parameter-for-parameter, so each
//definition matches its declaration in user32.h.
//
//Return conventions (U1/U2): both return BOOL as a real success flag, so no Win32ErrorHandler and no CheckReturn, and the
//flag is passed through for the caller to read. GetUserObjectSecurity reports a too-small buffer only through its lpnLengthNeeded
//out-parameter, which the wrapper forwards as the caller wrote it and does not inspect - the same shape the U1 note makes
//of GetUserObjectInformationT above.
//
//Addendum, 11 Nov 2026 (U2 Small TU batch S1, SmallTUBatchRegionChecks/SmallTUBatchS1 item security sites): the
//paragraph above is T1's record and stands superseded - the endpoint table's rows GetUserObjectSecurity and
//SetUserObjectSecurity both now read checked with checkClass BoolCheck, so BOTH wrappers carry a CheckReturn and this
//TU gained its one ../returncheck.h include (0 -> 2 CheckReturn sites in this TU, 1 of the library's 5 seeds plus this
//batch's 8). The decision itself is the table's, not re-derived here; the site comments below carry each row's verbatim
//docQuote and the class it names.

//Gate: standalone security.cpp at /W4 exit 0, then the whole folder 46/46 at /W4 exit 0, dated by tmp/obj/security.obj
//being newer than the source.

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    //The handle contract is the part a caller can get wrong, so it is stated, not coded. hObj must name a window-station
    //or desktop object (an HWINSTA or HDESK from the family above); pSIRequested selects which security bits the caller
    //wants, pSD is the buffer they are read into and nLength its capacity, while lpnLengthNeeded is the out-parameter
    //that carries the required length when pSD is too small. None of that is visible to a wrapper, so all five arguments
    //are forwarded as the caller wrote them and the BOOL answer is passed through unchanged.
    //Return policy (U2, table row GetUserObjectSecurity / security.cpp, 11 Nov 2026): the row reads checked with
    //checkClass BoolCheck. The table's verbatim docQuote - "If the function succeeds, the function returns nonzero. If
    //the function fails, it returns zero. To get extended error information, call GetLastError ." - reserves zero for
    //failure, and the only failure the caller can read here (a too-small pSD buffer) is reported through lpnLengthNeeded
    //IN ADDITION to that zero, never instead of it, so no success answer is zero and the session-27 soundness test
    //passes at every value. BOOL + zero-means-failure takes BoolCheck (returncheck.h:94), which already exists; G6
    //keeps the check live on every call, so a zero now raises a continuable issue the Win32ErrorHandler above reports
    //instead of travelling as a success. The row carries 0 in-tree callers (D14 published surface).
    BOOL User32::GetUserObjectSecurity(HANDLE hObj, PSECURITY_INFORMATION pSIRequested, PSECURITY_DESCRIPTOR pSD, DWORD nLength, LPDWORD lpnLengthNeeded)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, GetUserObjectSecurity);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, PSECURITY_INFORMATION, PSECURITY_DESCRIPTOR, DWORD, LPDWORD >(pFunc, hObj, pSIRequested, pSD, nLength, lpnLengthNeeded);
        return bResult;
    }

    //BOOL as a real success flag again. Here pSID points at the SECURITY_DESCRIPTOR to apply, and the OS copies it into
    //the object named by hObj; a caller that passes a handle for an unrelated object gets an OS failure it reads from
    //the return, which the layer neither sees nor corrects.
    //Return policy (U2, table row SetUserObjectSecurity / security.cpp, 11 Nov 2026): the row reads checked with
    //checkClass BoolCheck - the same shape as the GetUserObjectSecurity row above. The table's verbatim docQuote -
    //"If the function succeeds, the function returns nonzero. If the function fails, it returns zero. To get extended
    //error information, call GetLastError ." - reserves zero for failure and this member answers no value other than
    //that success/failure pair, so the session-27 soundness test passes at every value and BOOL + zero-means-failure
    //takes the existing BoolCheck (returncheck.h:94). G6 keeps the check live on every call, so a failed grant now
    //raises a continuable issue instead of being indistinguishable from a granted one. 0 in-tree callers (D14).
    BOOL User32::SetUserObjectSecurity(HANDLE hObj, PSECURITY_INFORMATION pSIRequested, PSECURITY_DESCRIPTOR pSID)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, SetUserObjectSecurity);
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, HANDLE, PSECURITY_INFORMATION, PSECURITY_DESCRIPTOR >(pFunc, hObj, pSIRequested, pSID);
        return bResult;
    }

}}}
