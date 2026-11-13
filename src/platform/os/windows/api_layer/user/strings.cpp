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

//=== api_layer conventions ========================================= Added 8 Nov 2026
//   (T9 step 9a: the "//String functions" section declared 34 members non-static and no TU defined any of them - new TU)
//
//T9 (registers/tasks.md, D26) is the tranche queue for U4's remaining declared-but-undefined members, and this TU lands
//step 9a's two halves together, the way T1's security.cpp, T3's powermanagement.cpp, T4's help.cpp, T5's ime.cpp and
//T6's listbox.cpp did: it defines all 34 members of user32.h's "//String functions" (user32.h:342-375), and user32.h
//gains `static` on all 34 declarations. U4's link gap (a caller failed at LNK2019) and U18's non-static shape close
//together. Coverage stays 759 of 1,018 - all 34 were already declared, so the instrument counted them; this closes a
//link gap, never a coverage gap (the arithmetic D24 recorded for T6).
//
//Binding - 34 x qor_pp_useswinapi(user32, <literal>) and 0 x qor_pp_useswinapiAW. 32 of the 34 are the two halves of an
//A/W export pair and 2 (CharNextExA, CharPrevExA) are single exports - the loaded user32.dll table has no CharNextExW
//row and no CharPrevExW row. All 34 literals are present in that table, so no binding here can throw
//"Missing library function." The ordinals: CharLowerA 2E CharLowerW 31 CharLowerBuffA 2F CharLowerBuffW 30 CharNextA 32
//CharNextExA 33 CharNextW 34 CharPrevA 35 CharPrevExA 36 CharPrevW 37 CharToOemA 38 CharToOemBuffA 39 CharToOemBuffW 3A
//CharToOemW 3B CharUpperA 3C CharUpperBuffA 3D CharUpperBuffW 3E CharUpperW 3F IsCharAlphaA 227 IsCharAlphaNumericA 228
//IsCharAlphaNumericW 229 IsCharAlphaW 22A IsCharLowerA 22B IsCharLowerW 22C IsCharUpperA 22D IsCharUpperW 22E LoadStringA
//267 LoadStringW 268 OemToCharA 297 OemToCharBuffA 298 OemToCharBuffW 299 OemToCharW 29A wvsprintfA 3F8 wvsprintfW 3F9.
//
//D13 (docs/agent/decisions/D13.md) asks for the spelling that states what GetProcAddress is given. The wrappers here
//carry the variant suffix themselves - the members are CharLowerA/CharLowerW, not CharLowerT - so the binding literal
//already IS the exported name, and qor_pp_useswinapiAW would be wrong: it pastes Stem##W (kernel32.h:77) and would
//bind the member CharLowerA to the export CharLowerW. The winuser.h stem switches ('#define CharLower  CharLowerW' at
//:5724-:5727, and the same shape after each of the other A/W pairs) cannot reach these literals - not one of the 34
//names is itself a macro - so the A/W choice is stated by the source and not by the UNICODE switch. The sibling layers
//spell variant-suffixed wrappers the same way (kconsole.cpp:624/633 WriteConsoleA/WriteConsoleW, kenviron.cpp:21-48
//GetEnvironmentStringsA/W, fileops.cpp:26/35 SHFileOperationA/W).
//
//SDK availability, read at the declaration line in um\WinUser.h 10.0.26100.0 (the header the gate's /showIncludes
//resolves): all 34 sit under #ifndef NOUSER (:64). The partition differs by group - CharToOemA/W :5598/:5604, OemToCharA/W
//:5617/:5624, CharToOemBuffA/W :5636/:5643 and OemToCharBuffA/W :5656/:5663 under
//WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) at :5388; CharUpperA/W :5682/:5687, CharUpperBuffA/W :5698/:5704,
//CharLowerA/W :5716/:5721, CharLowerBuffA/W :5732/:5738, CharNextA/W :5750/:5755 and CharPrevA/W :5766/:5772 under
//DESKTOP | SYSTEM at :5677, with CharNextExA :5785 and CharPrevExA :5793 additionally inside #if(WINVER >= 0x0400) at
//:5781; the eight IsChar* :5828-:5881 under DESKTOP | SYSTEM at :5818 and additionally #ifndef NOLANGUAGE at :5820;
//LoadStringA/W :11181/:11189 under #ifndef NOAPISET at :11173 and DESKTOP at :11176; wvsprintfA/W :306/:313 under
//DESKTOP at :296. kernel32.h:22-30 pins WINVER/_WIN32_WINNT at 0x0A00 (D21), so all 34 prototypes are visible and
//user32.h needs no guard of its own; NOUSER/NOLANGUAGE/NOAPISET stay unpinned shrink macros, so a consumer that defines
//one loses the prototype - the half of U16 the pin did not close.
//
//Signatures - D18's rule, the published signature is the prototype of the export this member binds, parameter names
//included, and user32.h already spells all 34 with the SDK's names, so the header edit is `static` only: no type and no
//parameter name moves, so no mangled name moves and no caller follows. CharLowerA/W and CharUpperA/W are _Inout_ on one
//character and hand the same pointer back; CharLowerBuffA/W and CharUpperBuffA/W are _Inout_updates_(cchLength) and return
//the number of characters folded; CharNextA/W/ExA and CharPrevA/W/ExA are _In_; CharToOemA/W and OemToCharA/W take an _In_
//source and an _Out_writes_(_Inexpressible_(strlen(pSrc)+1)) destination, so the caller must size it - winuser.h:5617 and
//:5624 attach __drv_preferredFunction("OemToCharBuff","Does not validate buffer size") to OemToCharA/W, and these wrappers
//pass the destination through unchecked exactly as the SDK does; the Buff pairs are _Out_writes_(cchDstLength); LoadStringA/W
//take _In_opt_ HINSTANCE and _Out_writes_to_(cchBufferMax,return+1) and return the count read; wvsprintfA/W take _Out_
//output, an _In_ _Printf_format_string_ format and the va_list, forwarded with the type the SDK declares.
//
//Record: the 34 declarations needed only `static`, so this TU is the whole of step 9a. The section is 34/34 declared,
//34/34 defined and 34/34 bound; the folder moves 47 -> 48 sources, 548 -> 582 static, 86 -> 52 non-static and 547 -> 581
//bindings over the close (_tmp_static_census.ps1).

namespace qor { namespace win { namespace api {


    //Return policy (U2, table row CharLowerA / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare and carries no token - docQuote "Type: LPTSTR If the operand is a character string, the function returns
    //a pointer to the converted string. Because the string is converted in place, the return value is equal to lpsz ." is a
    //value answer with no reserved failure value, and the page states it outright: "There is no indication of success or
    //failure. Failure is rare. There is no extended error information for this function; do not call GetLastError ." - so
    //D51 step 2 (an API that reserves no return value for errors) reads no-check and keying a class on the returned pointer
    //would report the in-place conversion itself as a failure; AUDIT: 0 in-tree callers for the exact stem.
    LPSTR User32::CharLowerA(LPSTR lpsz)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharLowerA);
        return Library::Call<LPSTR, LPSTR>(pFunc, lpsz);
    }

    //Return policy (U2, table row CharLowerW / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - the CharLowerA row's page text word for word, with the wide parameter name, so the same reading applies:
    //the return is the in-place conversion's own pointer and the page names no failure value ("do not call GetLastError").
    //AUDIT: 0 in-tree callers for the exact stem.
    LPWSTR User32::CharLowerW(LPWSTR lpwsz)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharLowerW);
        return Library::Call<LPWSTR, LPWSTR>(pFunc, lpwsz);
    }

    //Return policy (U2, table row CharLowerBuffA / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "Type: DWORD The return value is the number of characters processed. For example, if CharLowerBuff
    //(\"Acme of Operating Systems\", 10) succeeds, the return value is 10." is a count answer, and a cchLength of 0 is a legal
    //call answering 0, so no value is reserved for failure and CheckNonZero< DWORD > would report a legal empty-buffer call
    //as a failure; the fold itself lands in the _Inout_ buffer, not in the return. AUDIT: 0 in-tree callers for the exact stem.
    DWORD User32::CharLowerBuffA(LPSTR lpsz, DWORD cchLength)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharLowerBuffA);
        return Library::Call<DWORD, LPSTR, DWORD>(pFunc, lpsz, cchLength);
    }

    //Return policy (U2, table row CharLowerBuffW / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - the CharLowerBuffA row's Return value section word for word over the wide buffer, so the same count
    //reading applies and the documented zero for a zero-length call is a legal answer; deliberately unlike the CharToOemBuff
    //pair below, whose pages do name a failure value for the same DWORD-shaped call. AUDIT: 0 in-tree callers for the exact stem.
    DWORD User32::CharLowerBuffW(LPWSTR lpwsz, DWORD cchLength)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharLowerBuffW);
        return Library::Call<DWORD, LPWSTR, DWORD>(pFunc, lpwsz, cchLength);
    }

    //CharNextA/W step one character forwards, honouring a multibyte lead in the narrow case; they take a
    //const pointer because they do not write through it, unlike CharLowerA/W.
    //Return policy (U2, table row CharNextA / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "Type: LPTSTR The return value is a pointer to the next character in the string, or to the
    //terminating null character if at the end of the string. If lpsz points to the terminating null character, the return
    //value is equal to lpsz ." is a position answer, and pointing at the terminator is the state the caller is stepping
    //through to, so nothing is reserved for failure; the page names no failure value at all. AUDIT: 0 in-tree callers.
    LPSTR User32::CharNextA(LPCSTR lpsz)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharNextA);
        return Library::Call<LPSTR, LPCSTR>(pFunc, lpsz);
    }

    //Return policy (U2, table row CharNextW / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - the CharNextA row's Return value section word for word with the wide parameter name, so the same
    //position-answer reading applies and nothing is reserved for failure. AUDIT: 0 in-tree callers for the exact stem.
    LPWSTR User32::CharNextW(LPCWSTR lpwsz)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharNextW);
        return Library::Call<LPWSTR, LPCWSTR>(pFunc, lpwsz);
    }
    //The Ex pair works on one character of a supplied code page rather than the process default, hence the
    //WORD CodePage first parameter. user32.dll exports no wide form of either.
    //Return policy (U2, table row CharNextExA / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "Type: LPSTR The return value is a pointer to the next character in the string, or to the
    //terminating null character if at the end of the string. If lpCurrentChar points to the terminating null character,
    //the return value is equal to lpCurrentChar ." is the CharNextA row's answer over a supplied code page, so the same
    //reading applies; this is one of the two single exports in this TU (user32.dll has no CharNextExW row). AUDIT: 0 in-tree callers.
    LPSTR User32::CharNextExA(WORD CodePage, LPCSTR lpCurrentChar, DWORD dwFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharNextExA);
        return Library::Call<LPSTR, WORD, LPCSTR, DWORD>(pFunc, CodePage, lpCurrentChar, dwFlags);
    }

    //Return policy (U2, table row CharPrevA / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "Type: LPTSTR The return value is a pointer to the preceding character in the string, or to
    //the first character in the string if the lpszCurrent parameter equals the lpszStart parameter." is a position answer
    //whose only stated outcome is clamping to the start, so every return is a legal answer and the page names no failure
    //value at all - D51 step 2 reads no-check. AUDIT: 0 in-tree callers for the exact stem.
    LPSTR User32::CharPrevA(LPCSTR lpszStart, LPCSTR lpszCurrent)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharPrevA);
        return Library::Call<LPSTR, LPCSTR, LPCSTR>(pFunc, lpszStart, lpszCurrent);
    }

    //Return policy (U2, table row CharPrevW / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - the CharPrevA row's Return value section word for word with the wide parameter names, so the same
    //position-answer reading applies. AUDIT: 0 in-tree callers for the exact stem.
    LPWSTR User32::CharPrevW(LPCWSTR lpwszStart, LPCWSTR lpwszCurrent)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharPrevW);
        return Library::Call<LPWSTR, LPCWSTR, LPCWSTR>(pFunc, lpwszStart, lpwszCurrent);
    }

    //Return policy (U2, table row CharPrevExA / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - docQuote "Type: LPSTR The return value is a pointer to the preceding character in the string, or to
    //the first character in the string if the lpCurrentChar parameter equals the lpStart parameter." is the CharPrevA row's
    //position answer over a supplied code page; the page names no failure value, and this is the TU's other single export.
    //AUDIT: 0 in-tree callers for the exact stem.
    LPSTR User32::CharPrevExA(WORD CodePage, LPCSTR lpStart, LPCSTR lpCurrentChar, DWORD dwFlags)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharPrevExA);
        return Library::Call<LPSTR, WORD, LPCSTR, LPCSTR, DWORD>(pFunc, CodePage, lpStart, lpCurrentChar, dwFlags);
    }

    //CharToOemA/W convert an ANSI or wide source into the console code page; the destination is narrow in
    //both cases, which is why CharToOemW takes a wide source and a narrow destination.
    //Return policy (U2, table row CharToOemA / strings.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - docQuote "The return value is always nonzero except when you pass the same address to
    //lpszSrc and lpszDst in the wide-character version of the function. In this case the function returns zero and
    //GetLastError returns ERROR_INVALID_ADDRESS ." makes zero the aliased-buffer failure rather than an answer, so zero is
    //never a legal result here and the converted string lands in the lpszDst out-parameter; sdkRetType BOOL so the class
    //base is BOOL and BoolCheck (returncheck.h:94) covers it; unlike the CharLower/CharUpper family the page names
    //GetLastError. AUDIT: 0 in-tree callers for the exact stem.
    BOOL User32::CharToOemA(LPCSTR lpszSrc, LPSTR lpszDst)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharToOemA);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, LPCSTR, LPSTR >(pFunc, lpszSrc, lpszDst);
        return hResult;
    }

    //Return policy (U2, table row CharToOemW / strings.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - the CharToOemA row's Return value section word for word, and this is the wide-character
    //version the text's own caveat names ("in the wide-character version of the function ... the function returns zero and
    //GetLastError returns ERROR_INVALID_ADDRESS"), so zero is reserved for the aliased lpszSrc/lpszDst call; sdkRetType
    //BOOL so BoolCheck (returncheck.h:94) covers it. AUDIT: 0 in-tree callers for the exact stem.
    BOOL User32::CharToOemW(LPCWSTR lpwszSrc, LPSTR lpszDst)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharToOemW);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, LPCWSTR, LPSTR >(pFunc, lpwszSrc, lpszDst);
        return hResult;
    }

    //The Buff forms take the destination length, so they validate it - unlike CharToOemA/W above they cannot
    //run past the caller's buffer.
    //Return policy (U2, table row CharToOemBuffA / strings.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //so the call carries the token - the CharToOemA row's Return value section word for word, "always nonzero except when
    //you pass the same address to lpszSrc and lpszDst ... the function returns zero and GetLastError returns
    //ERROR_INVALID_ADDRESS", so zero is reserved and the converted buffer is the lpszDst out-parameter; sdkRetType
    //BOOL so BoolCheck (returncheck.h:94) covers it. AUDIT: 0 in-tree callers for the exact stem.
    BOOL User32::CharToOemBuffA(LPCSTR lpszSrc, LPSTR lpszDst, DWORD cchDstLength)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharToOemBuffA);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, LPCSTR, LPSTR, DWORD >(pFunc, lpszSrc, lpszDst, cchDstLength);
        return hResult;
    }

    //Return policy (U2, table row CharToOemBuffW / strings.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck,
    //so the call carries the token - the CharToOemBuffA row's Return value section word for word (the Unicode twin), zero
    //reserved for the aliased lpszSrc/lpszDst call rather than being a legal answer; sdkRetType BOOL so BoolCheck
    //(returncheck.h:94) covers it. AUDIT: 0 in-tree callers for the exact stem.
    BOOL User32::CharToOemBuffW(LPCWSTR lpwszSrc, LPSTR lpszDst, DWORD cchDstLength)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharToOemBuffW);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, LPCWSTR, LPSTR, DWORD >(pFunc, lpwszSrc, lpszDst, cchDstLength);
        return hResult;
    }

    //Return policy (U2, table row CharUpperA / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - the CharLowerA row's Return value section word for word - "There is no indication of success or
    //failure. Failure is rare. There is no extended error information for this function; do not call GetLastError ." -
    //the page states no success/failure signal and forbids GetLastError, so D51 step 2 reads no-check and the return is
    //the converted operand (equal to lpsz), a legal answer at every value; sdkRetType blank, pubRetType LPSTR.
    //AUDIT: 0 in-tree callers for the exact stem.
    LPSTR User32::CharUpperA(LPSTR lpsz)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharUpperA);
        return Library::Call<LPSTR, LPSTR>(pFunc, lpsz);
    }

    //Return policy (U2, table row CharUpperW / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - the CharUpperA row's Return value section word for word with the wide parameter name (the Unicode
    //twin), so the same reading applies: the page states no success/failure signal and forbids GetLastError, and the
    //return is the converted operand; sdkRetType blank, pubRetType LPWSTR. AUDIT: 0 in-tree callers for the exact stem.
    LPWSTR User32::CharUpperW(LPWSTR lpwsz)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharUpperW);
        return Library::Call<LPWSTR, LPWSTR>(pFunc, lpwsz);
    }

    //Return policy (U2, table row CharUpperBuffA / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so
    //the call stays bare - value answer with no failure value named, docQuote "The return value is the number of characters
    //processed. For example, if CharUpperBuff (\"Zenith of API Sets\", 10) succeeds, the return value is 10." - the return is
    //the count of characters converted rather than a failure signal, so a zero-length call answering 0 is a legal answer
    //and CheckNonZero< DWORD > would report it as a failure; the CharLowerBuffA row's reading with CharUpperBuff as the
    //example; sdkRetType DWORD. AUDIT: 0 in-tree callers for the exact stem.
    DWORD User32::CharUpperBuffA(LPSTR lpsz, DWORD cchLength)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharUpperBuffA);
        return Library::Call<DWORD, LPSTR, DWORD>(pFunc, lpsz, cchLength);
    }

    //Return policy (U2, table row CharUpperBuffW / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so
    //the call stays bare - the CharUpperBuffA row's Return value section word for word (the Unicode twin), so the same
    //count-answer reading applies and nothing is reserved for failure; sdkRetType DWORD. AUDIT: 0 in-tree callers
    //for the exact stem.
    DWORD User32::CharUpperBuffW(LPWSTR lpwsz, DWORD cchLength)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, CharUpperBuffW);
        return Library::Call<DWORD, LPWSTR, DWORD>(pFunc, lpwsz, cchLength);
    }

    //The IsChar* predicates take one character by value. winuser.h:5828-5881 declares the narrow form _In_ CHAR
    //and the wide form _In_ WCHAR; user32.h keeps the QOR spellings char and wchar_t, and the SDK parameter is
    //read as a character value, so passing a wchar_t to the WCHAR slot is the same conversion the SDK headers
    //make at their own call sites.
    //Return policy (U2, table row IsCharAlphaA / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so
    //the call stays bare - a question, not a flag: docQuote "If the character is alphabetical, the return value is nonzero.
    //If the character is not alphabetical, the return value is zero. To get extended error information, call GetLastError ."
    //has the zero answering the caller's question about the character it passed (a digit or punctuation is a legal
    //non-alphabetic character), so nothing is reserved and keying BoolCheck would report a legal answer as a failure;
    //same shape as the IsClipboardFormatAvailable, IsWindowArranged and TranslateMessage rows, and deliberately unlike
    //the CharToOem family above, whose zero is the aliased-buffer failure rather than an answer; sdkRetType BOOL.
    //AUDIT: 0 in-tree callers for the exact stem.
    BOOL User32::IsCharAlphaA(char ch)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsCharAlphaA);
        return Library::Call<BOOL, char>(pFunc, ch);
    }

    //Return policy (U2, table row IsCharAlphaW / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - the IsCharAlphaA row's Return value section word for word (the Unicode twin), so the zero answers
    //the caller's question about the character and no value is reserved; sdkRetType BOOL. AUDIT: 0 in-tree callers
    //for the exact stem.
    BOOL User32::IsCharAlphaW(wchar_t wch)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsCharAlphaW);
        return Library::Call<BOOL, wchar_t>(pFunc, wch);
    }

    //Return policy (U2, table row IsCharAlphaNumericA / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so
    //the call stays bare - a question, not a flag: docQuote "If the character is alphanumeric, the return value is nonzero.
    //If the character is not alphanumeric, the return value is zero. To get extended error information, call GetLastError ."
    //has the zero answering the caller's question about the character it passed (punctuation is a legal non-alphanumeric
    //character), so nothing is reserved and keying BoolCheck would report a legal answer as a failure; the IsCharAlphaA
    //row's shape with "alphanumeric" in place of "alphabetical"; sdkRetType BOOL. AUDIT: 0 in-tree callers for the exact stem.
    BOOL User32::IsCharAlphaNumericA(char ch)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsCharAlphaNumericA);
        return Library::Call<BOOL, char>(pFunc, ch);
    }

    //Return policy (U2, table row IsCharAlphaNumericW / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so
    //the call stays bare - the IsCharAlphaNumericA row's Return value section word for word (the Unicode twin), so the zero
    //answers the caller's question about the character and no value is reserved; sdkRetType BOOL. AUDIT: 0 in-tree callers
    //for the exact stem.
    BOOL User32::IsCharAlphaNumericW(wchar_t wch)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsCharAlphaNumericW);
        return Library::Call<BOOL, wchar_t>(pFunc, wch);
    }

    //Return policy (U2, table row IsCharLowerA / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - a question, not a flag: docQuote "If the character is lowercase, the return value is nonzero. If the
    //character is not lowercase, the return value is zero. To get extended error information, call GetLastError ." has the
    //zero answering the caller's question, an uppercase or digit character being a legal state answering zero, so nothing
    //is reserved; sdkRetType BOOL. AUDIT: 0 in-tree callers for the exact stem.
    BOOL User32::IsCharLowerA(char ch)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsCharLowerA);
        return Library::Call<BOOL, char>(pFunc, ch);
    }

    //Return policy (U2, table row IsCharLowerW / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - the IsCharLowerA row's Return value section word for word (the Unicode twin), so the same reading
    //applies and no value is reserved; sdkRetType BOOL. AUDIT: 0 in-tree callers for the exact stem.
    BOOL User32::IsCharLowerW(wchar_t wch)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsCharLowerW);
        return Library::Call<BOOL, wchar_t>(pFunc, wch);
    }

    //Return policy (U2, table row IsCharUpperA / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - a question, not a flag: docQuote "If the character is uppercase, the return value is nonzero. If the
    //character is not uppercase, the return value is zero. To get extended error information, call GetLastError ." has the
    //zero answering the caller's question about the character it passed, so nothing is reserved and keying BoolCheck would
    //report a legal answer as a failure; sdkRetType BOOL. AUDIT: 0 in-tree callers for the exact stem.
    BOOL User32::IsCharUpperA(char ch)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsCharUpperA);
        return Library::Call<BOOL, char>(pFunc, ch);
    }

    //Return policy (U2, table row IsCharUpperW / strings.cpp, 12 Nov 2026): the row reads unchecked with no checkClass, so the
    //call stays bare - the IsCharUpperA row's Return value section word for word (the Unicode twin), so the zero answers
    //the caller's question about the character and no value is reserved; sdkRetType BOOL. AUDIT: 0 in-tree callers
    //for the exact stem.
    BOOL User32::IsCharUpperW(wchar_t wch)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, IsCharUpperW);
        return Library::Call<BOOL, wchar_t>(pFunc, wch);
    }
    //LoadStringA/W copy one resource-string entry into the caller's buffer and return the number of
    //characters copied (0 means missing or too large); the SDK marks the HINSTANCE _In_opt_, so NULL is
    //a legal handle meaning "use the currently executing module".
    //Return policy (U2, table row LoadStringA / strings.cpp, 12 Nov 2026): the row reads checked with checkClass CheckNonZero< int >,
    //so the call carries the token - docQuote "If the function succeeds, the return value is the number of characters copied
    //into the buffer, not including the terminating null character. If the string resource does not exist, the return value
    //is zero. To get extended error information, call GetLastError ." names zero as what a missing resource answers and never
    //documents a zero-length resource as a legal success answer, so zero is reserved and the count is what the caller uses
    //to terminate the buffer; sdkRetType int so the base is int and CheckNonZero< int > (returncheck.h:187) fits. CAVEAT
    //recorded: the page does not rule out an empty resource, so a caller that loads one would see this check fire - flagged
    //for the code-side leaf rather than decided here. AUDIT: 0 in-tree callers for the exact stem.
    int User32::LoadStringA(HINSTANCE hInstance, UINT uID, LPSTR lpBuffer, int nBufferMax)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, LoadStringA);
        CheckReturn< int, CheckNonZero< int > >::TType hResult = Library::Call< int, HINSTANCE, UINT, LPSTR, int >(pFunc, hInstance, uID, lpBuffer, nBufferMax);
        return hResult;
    }

    //Return policy (U2, table row LoadStringW / strings.cpp, 12 Nov 2026): the row reads checked with checkClass CheckNonZero< int >,
    //so the call carries the token - the LoadStringA row's reading with the wider success set spelled out: "If the function
    //succeeds, the return value is one of the following: The number of characters copied into the buffer (if cchBufferMax
    //is non-zero), not including the terminating null character. The number of characters in the string resource that lpBuffer
    //points to (if cchBufferMax is zero). ... Zero if the string resource does not exist." - the cchBufferMax-is-zero branch
    //returns the resource's own length rather than a truncation count, so a zero return is only ever the documented missing-resource
    //case; sdkRetType int so CheckNonZero< int > (returncheck.h:187) fits; same empty-resource caveat as the LoadStringA row.
    //AUDIT: 0 in-tree callers for the exact stem.
    int User32::LoadStringW(HINSTANCE hInstance, UINT uID, LPWSTR lpBuffer, int nBufferMax)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, LoadStringW);
        CheckReturn< int, CheckNonZero< int > >::TType hResult = Library::Call< int, HINSTANCE, UINT, LPWSTR, int >(pFunc, hInstance, uID, lpBuffer, nBufferMax);
        return hResult;
    }

    //OemToCharA/W are the inverse of CharToOemA/W: the narrow source is read in the console code page and
    //written back as ANSI/wide. These are the pair the SDK flags with
    //__drv_preferredFunction("OemToCharBuff","Does not validate buffer size"), so the unbounded forms are
    //kept only because user32.h declares them - prefer the Buff pair below when the length is known.
    //Return policy (U2, table row OemToCharA / strings.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - the CharToOemA row's Return value section word for word, "The return value is always nonzero
    //except when you pass the same address to lpszSrc and lpszDst in the wide-character version of the function. In this case
    //the function returns zero and GetLastError returns ERROR_INVALID_ADDRESS ." - zero reserved for the aliased-buffer call,
    //the converted string lands in the lpszDst out-parameter; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it.
    //AUDIT: 0 in-tree callers for the exact stem.
    BOOL User32::OemToCharA(LPCSTR lpszSrc, LPSTR lpszDst)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, OemToCharA);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, LPCSTR, LPSTR >(pFunc, lpszSrc, lpszDst);
        return hResult;
    }

    //Return policy (U2, table row OemToCharW / strings.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - the page's Return value section word for word, "The return value is always nonzero
    //except when you pass the same address to lpszSrc and lpszDst in the wide-character version of the function. In this case
    //the function returns zero and GetLastError returns ERROR_INVALID_ADDRESS ." - and this stem IS the wide-character version
    //the sentence names, so the aliased-buffer call (lpszSrc read as ANSI into the wide destination) answers zero and
    //ERROR_INVALID_ADDRESS ; zero is reserved, the converted string lands in the lpwszDst out-parameter, and no legal zero
    //is documented; sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it.
    //AUDIT: 0 in-tree callers for the exact stem.
    BOOL User32::OemToCharW(LPCSTR lpszSrc, LPWSTR lpwszDst)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, OemToCharW);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, LPCSTR, LPWSTR >(pFunc, lpszSrc, lpwszDst);
        return hResult;
    }

    //Return policy (U2, table row OemToCharBuffA / strings.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - the CharToOemBuffA row's Return value section word for word, "The return value is always nonzero
    //except when you pass the same address to lpszSrc and lpszDst in the wide-character version of the function. In this case
    //the function returns zero and GetLastError returns ERROR_INVALID_ADDRESS ." - the exception the sentence names is the
    //wide-character version, so for this ANSI twin the return is documented as always nonzero on success and zero is reserved
    //outright; the converted buffer lands in the lpszDst out-parameter, so the BOOL carries no answer; sdkRetType BOOL so
    //BoolCheck (returncheck.h:94) covers it.
    //AUDIT: 0 in-tree callers for the exact stem.
    BOOL User32::OemToCharBuffA(LPCSTR lpszSrc, LPSTR lpszDst, DWORD cchDstLength)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, OemToCharBuffA);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, LPCSTR, LPSTR, DWORD >(pFunc, lpszSrc, lpszDst, cchDstLength);
        return hResult;
    }

    //Return policy (U2, table row OemToCharBuffW / strings.cpp, 12 Nov 2026): the row reads checked with checkClass BoolCheck, so
    //the call carries the token - the OemToCharBuffA row's Return value section word for word (the Unicode twin), and the shared
    //text's caveat names this wide-character version as the one where the aliased-buffer call answers zero: "The return value is
    //always nonzero except when you pass the same address to lpszSrc and lpszDst in the wide-character version of the function.
    //In this case the function returns zero and GetLastError returns ERROR_INVALID_ADDRESS ." - zero reserved, the converted
    //buffer landing in the lpwszDst out-parameter; cchDstLength is the buffer length the caller passes, not the return;
    //sdkRetType BOOL so BoolCheck (returncheck.h:94) covers it.
    //AUDIT: 0 in-tree callers for the exact stem.
    BOOL User32::OemToCharBuffW(LPCWSTR lpwszSrc, LPWSTR lpwszDst, DWORD cchDstLength)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, OemToCharBuffW);
        CheckReturn< BOOL, BoolCheck >::TType hResult = Library::Call< BOOL, LPCWSTR, LPWSTR, DWORD >(pFunc, lpwszSrc, lpwszDst, cchDstLength);
        return hResult;
    }

    //Return policy (U2, table row wvsprintfA / strings.cpp, 12 Nov 2026): the row reads unchecked, so the call carries
    //no token and keeps its bare return - "Type: int If the function succeeds, the return value is the number of characters
    //stored in the buffer, not counting the terminating null character. If the function fails, the return value is less than
    //the length of the expected output. To get extended error information, call GetLastError ." - the failure signal is a
    //comparison against a length the page never states and the call never passes (this is the unbounded vsprintf shape:
    //there is no buffer-length argument, which is why the SDK marks the destination _Out_ rather than _Inout_), so no value
    //is reserved and no class in returncheck.h can key on it - TRangeCheck needs static Min/Max and the expected length is
    //caller-specific. 0 in-tree callers (change-safe).
    //wvsprintfA/W format into the caller's buffer with no length argument, so they can overrun it - the
    //SDK marks the destination _Out_ and the format string _Printf_format_string_ for SAL. The va_list is
    //forwarded with the same type the SDK declares, so the callee sees exactly what a direct call passes.
    int User32::wvsprintfA(LPSTR lpOutput, LPCSTR lpFmt, va_list arglist)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, wvsprintfA);
        return Library::Call<int, LPSTR, LPCSTR, va_list>(pFunc, lpOutput, lpFmt, arglist);
    }

    //Return policy (U2, table row wvsprintfW / strings.cpp, 12 Nov 2026): the row reads unchecked, so the call carries
    //no token and keeps its bare return - the wvsprintfA row's Return value section word for word (the Unicode twin):
    //"If the function succeeds, the return value is the number of characters stored in the buffer, not counting the
    //terminating null character. If the function fails, the return value is less than the length of the expected output." -
    //the same reading applies: the page names a comparison against the caller's expected length rather than a reserved
    //value, and the call takes no buffer-length argument, so a short count is reported only relative to what the caller
    //wanted and no class in returncheck.h fits. 0 in-tree callers (change-safe).
    int User32::wvsprintfW(LPWSTR lpOutput, LPCWSTR lpFmt, va_list arglist)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(user32, wvsprintfW);
        return Library::Call<int, LPWSTR, LPCWSTR, va_list>(pFunc, lpOutput, lpFmt, arglist);
    }

}}}   //qor::win::api
