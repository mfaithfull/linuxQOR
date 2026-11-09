// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "user32.h"
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


    LPSTR User32::CharLowerA(LPSTR lpsz)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharLowerA);
        return Library::Call<LPSTR, LPSTR>(pFunc, lpsz);
    }

    LPWSTR User32::CharLowerW(LPWSTR lpwsz)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharLowerW);
        return Library::Call<LPWSTR, LPWSTR>(pFunc, lpwsz);
    }

    DWORD User32::CharLowerBuffA(LPSTR lpsz, DWORD cchLength)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharLowerBuffA);
        return Library::Call<DWORD, LPSTR, DWORD>(pFunc, lpsz, cchLength);
    }

    DWORD User32::CharLowerBuffW(LPWSTR lpwsz, DWORD cchLength)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharLowerBuffW);
        return Library::Call<DWORD, LPWSTR, DWORD>(pFunc, lpwsz, cchLength);
    }

    //CharNextA/W step one character forwards, honouring a multibyte lead in the narrow case; they take a
    //const pointer because they do not write through it, unlike CharLowerA/W.
    LPSTR User32::CharNextA(LPCSTR lpsz)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharNextA);
        return Library::Call<LPSTR, LPCSTR>(pFunc, lpsz);
    }

    LPWSTR User32::CharNextW(LPCWSTR lpwsz)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharNextW);
        return Library::Call<LPWSTR, LPCWSTR>(pFunc, lpwsz);
    }
    //The Ex pair works on one character of a supplied code page rather than the process default, hence the
    //WORD CodePage first parameter. user32.dll exports no wide form of either.
    LPSTR User32::CharNextExA(WORD CodePage, LPCSTR lpCurrentChar, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharNextExA);
        return Library::Call<LPSTR, WORD, LPCSTR, DWORD>(pFunc, CodePage, lpCurrentChar, dwFlags);
    }

    LPSTR User32::CharPrevA(LPCSTR lpszStart, LPCSTR lpszCurrent)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharPrevA);
        return Library::Call<LPSTR, LPCSTR, LPCSTR>(pFunc, lpszStart, lpszCurrent);
    }

    LPWSTR User32::CharPrevW(LPCWSTR lpwszStart, LPCWSTR lpwszCurrent)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharPrevW);
        return Library::Call<LPWSTR, LPCWSTR, LPCWSTR>(pFunc, lpwszStart, lpwszCurrent);
    }

    LPSTR User32::CharPrevExA(WORD CodePage, LPCSTR lpStart, LPCSTR lpCurrentChar, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharPrevExA);
        return Library::Call<LPSTR, WORD, LPCSTR, LPCSTR, DWORD>(pFunc, CodePage, lpStart, lpCurrentChar, dwFlags);
    }

    //CharToOemA/W convert an ANSI or wide source into the console code page; the destination is narrow in
    //both cases, which is why CharToOemW takes a wide source and a narrow destination.
    BOOL User32::CharToOemA(LPCSTR lpszSrc, LPSTR lpszDst)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharToOemA);
        return Library::Call<BOOL, LPCSTR, LPSTR>(pFunc, lpszSrc, lpszDst);
    }

    BOOL User32::CharToOemW(LPCWSTR lpwszSrc, LPSTR lpszDst)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharToOemW);
        return Library::Call<BOOL, LPCWSTR, LPSTR>(pFunc, lpwszSrc, lpszDst);
    }

    //The Buff forms take the destination length, so they validate it - unlike CharToOemA/W above they cannot
    //run past the caller's buffer.
    BOOL User32::CharToOemBuffA(LPCSTR lpszSrc, LPSTR lpszDst, DWORD cchDstLength)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharToOemBuffA);
        return Library::Call<BOOL, LPCSTR, LPSTR, DWORD>(pFunc, lpszSrc, lpszDst, cchDstLength);
    }

    BOOL User32::CharToOemBuffW(LPCWSTR lpwszSrc, LPSTR lpszDst, DWORD cchDstLength)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharToOemBuffW);
        return Library::Call<BOOL, LPCWSTR, LPSTR, DWORD>(pFunc, lpwszSrc, lpszDst, cchDstLength);
    }
    LPSTR User32::CharUpperA(LPSTR lpsz)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharUpperA);
        return Library::Call<LPSTR, LPSTR>(pFunc, lpsz);
    }

    LPWSTR User32::CharUpperW(LPWSTR lpwsz)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharUpperW);
        return Library::Call<LPWSTR, LPWSTR>(pFunc, lpwsz);
    }

    DWORD User32::CharUpperBuffA(LPSTR lpsz, DWORD cchLength)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharUpperBuffA);
        return Library::Call<DWORD, LPSTR, DWORD>(pFunc, lpsz, cchLength);
    }

    DWORD User32::CharUpperBuffW(LPWSTR lpwsz, DWORD cchLength)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CharUpperBuffW);
        return Library::Call<DWORD, LPWSTR, DWORD>(pFunc, lpwsz, cchLength);
    }

    //The IsChar* predicates take one character by value. winuser.h:5828-5881 declares the narrow form _In_ CHAR
    //and the wide form _In_ WCHAR; user32.h keeps the QOR spellings char and wchar_t, and the SDK parameter is
    //read as a character value, so passing a wchar_t to the WCHAR slot is the same conversion the SDK headers
    //make at their own call sites.
    BOOL User32::IsCharAlphaA(char ch)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsCharAlphaA);
        return Library::Call<BOOL, char>(pFunc, ch);
    }

    BOOL User32::IsCharAlphaW(wchar_t wch)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsCharAlphaW);
        return Library::Call<BOOL, wchar_t>(pFunc, wch);
    }

    BOOL User32::IsCharAlphaNumericA(char ch)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsCharAlphaNumericA);
        return Library::Call<BOOL, char>(pFunc, ch);
    }

    BOOL User32::IsCharAlphaNumericW(wchar_t wch)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsCharAlphaNumericW);
        return Library::Call<BOOL, wchar_t>(pFunc, wch);
    }

    BOOL User32::IsCharLowerA(char ch)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsCharLowerA);
        return Library::Call<BOOL, char>(pFunc, ch);
    }

    BOOL User32::IsCharLowerW(wchar_t wch)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsCharLowerW);
        return Library::Call<BOOL, wchar_t>(pFunc, wch);
    }

    BOOL User32::IsCharUpperA(char ch)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsCharUpperA);
        return Library::Call<BOOL, char>(pFunc, ch);
    }

    BOOL User32::IsCharUpperW(wchar_t wch)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsCharUpperW);
        return Library::Call<BOOL, wchar_t>(pFunc, wch);
    }
    //LoadStringA/W copy one resource-string entry into the caller's buffer and return the number of
    //characters copied (0 means missing or too large); the SDK marks the HINSTANCE _In_opt_, so NULL is
    //a legal handle meaning "use the currently executing module".
    int User32::LoadStringA(HINSTANCE hInstance, UINT uID, LPSTR lpBuffer, int nBufferMax)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, LoadStringA);
        return Library::Call<int, HINSTANCE, UINT, LPSTR, int>(pFunc, hInstance, uID, lpBuffer, nBufferMax);
    }

    int User32::LoadStringW(HINSTANCE hInstance, UINT uID, LPWSTR lpBuffer, int nBufferMax)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, LoadStringW);
        return Library::Call<int, HINSTANCE, UINT, LPWSTR, int>(pFunc, hInstance, uID, lpBuffer, nBufferMax);
    }

    //OemToCharA/W are the inverse of CharToOemA/W: the narrow source is read in the console code page and
    //written back as ANSI/wide. These are the pair the SDK flags with
    //__drv_preferredFunction("OemToCharBuff","Does not validate buffer size"), so the unbounded forms are
    //kept only because user32.h declares them - prefer the Buff pair below when the length is known.
    BOOL User32::OemToCharA(LPCSTR lpszSrc, LPSTR lpszDst)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, OemToCharA);
        return Library::Call<BOOL, LPCSTR, LPSTR>(pFunc, lpszSrc, lpszDst);
    }

    BOOL User32::OemToCharW(LPCSTR lpszSrc, LPWSTR lpwszDst)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, OemToCharW);
        return Library::Call<BOOL, LPCSTR, LPWSTR>(pFunc, lpszSrc, lpwszDst);
    }

    BOOL User32::OemToCharBuffA(LPCSTR lpszSrc, LPSTR lpszDst, DWORD cchDstLength)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, OemToCharBuffA);
        return Library::Call<BOOL, LPCSTR, LPSTR, DWORD>(pFunc, lpszSrc, lpszDst, cchDstLength);
    }

    BOOL User32::OemToCharBuffW(LPCWSTR lpwszSrc, LPWSTR lpwszDst, DWORD cchDstLength)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, OemToCharBuffW);
        return Library::Call<BOOL, LPCWSTR, LPWSTR, DWORD>(pFunc, lpwszSrc, lpwszDst, cchDstLength);
    }

    //wvsprintfA/W format into the caller's buffer with no length argument, so they can overrun it - the
    //SDK marks the destination _Out_ and the format string _Printf_format_string_ for SAL. The va_list is
    //forwarded with the same type the SDK declares, so the callee sees exactly what a direct call passes.
    int User32::wvsprintfA(LPSTR lpOutput, LPCSTR lpFmt, va_list arglist)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, wvsprintfA);
        return Library::Call<int, LPSTR, LPCSTR, va_list>(pFunc, lpOutput, lpFmt, arglist);
    }

    int User32::wvsprintfW(LPWSTR lpOutput, LPCWSTR lpFmt, va_list arglist)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, wvsprintfW);
        return Library::Call<int, LPWSTR, LPCWSTR, va_list>(pFunc, lpOutput, lpFmt, arglist);
    }

}}}   //qor::win::api
