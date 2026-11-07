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

    //Eleven wrappers - the whole "user32.h:446 //Rectangle functions" section: declared :447-:457, all static, all
    //defined here through the User32:: qualifier and in header order (in this TU: :97 CopyRect, :104 EqualRect,
    //:111 InflateRect, :118 IntersectRect, :125 IsRectEmpty, :132 OffsetRect, :139 PtInRect, :146 SetRect,
    //:153 SetRectEmpty, :160 SubtractRect, :167 UnionRect). 11/11 declarations pair with a definition here, so the
    //TU contributes 0 U4 (declared, never defined), 0 U11 (definition unqualified), 0 U13 (defined, declared
    //non-static), 0 U17 (A/W family without the folder's T suffix) and 0 U18 (return type contradicts the SDK) -
    //tmp/_tmp_rect_pair_pre_s28.txt (11/11 paired, 0 hdr-vs-def and 0 def-vs-sdk parameter-name divergences).
    //
    //Binding (D13): all eleven stems are single exports with no A/W family and no winuser.h #define for the stem, so
    //the bare qor_pp_useswinapi at :100, :107, :114, :121, :128, :135, :142, :149, :156, :163 and :170 is the only
    //spelling that can bind them at all - user32.dll exports CopyRect (5B / 00027820), EqualRect (10B / 00026060),
    //InflateRect (20C / 00008580), IntersectRect (223 / 000099C0), IsRectEmpty (23E / 00026330), OffsetRect
    //(29B / 0001BC30), PtInRect (2B8 / 00009D00), SetRect (360 / 000262C0), SetRectEmpty (361 / 00028AB0),
    //SubtractRect (3A3 / 00009250) and UnionRect (3C0 / 000289D0) in tmp/_tmp_user32_exports.txt, and cl /EP over the
    //TU emits exactly those eleven literals (tmp/_tmp_rect_pp_pre_s28.txt): 11/11 present in the export table, 0
    //macro, hence 0 P1, 0 P2 (no macro to rewrite the name, G1) and 0 P6 (no bare export beside an A/W family) -
    //tmp/_tmp_rect_class_pre_s28.txt. The folder census counts all eleven bindings here as resolvable (428 bindings,
    //1 unresolvable, at window.cpp:124) and the wrong-stem scan has no rectangle.cpp row
    //(tmp/_tmp_wrongstem_pre_s28.txt: 3 rows, all the known-benign cursor.cpp and resource.cpp ones).
    //
    //Signatures: eleven members against eleven SDK prototypes, every return BOOL - SetRect winuser.h:9752,
    //SetRectEmpty :9762, CopyRect :9768, InflateRect :9775, IntersectRect :9783, UnionRect :9791, SubtractRect
    //:9799, OffsetRect :9807, IsRectEmpty :9815, EqualRect :9821, PtInRect :9828 (tmp/_tmp_rect_ret_s28.ps1 - the
    //SDK puts the return type on its own line above the stem, session 22's caveat). Every parameter name matches the
    //SDK verbatim, and no call site has an arity error: tmp/_tmp_sigcmp.ps1 -Tu rectangle.cpp -First 447 -Last 457
    //gives 11/11 "decl==def ok" plus 4 call sites "ok" and 7 CALL TYPE MISMATCH rows holding 11 mismatched
    //arguments. All 11 are the known U25 spelling artifact - the definition writes CONST::RECT* where the template
    //argument writes CONST ::RECT* - and two of those rows carry a fourth spelling in the same template list,
    //CONST RECT* with no :: at all (in this TU: :108 and :171). Over code lines only the folder writes CONST:: 17,
    //CONST :: 18 and bare CONST 10 times (tmp/_tmp_rect_const_s28.ps1, which skips comment lines - otherwise a
    //contract block quoting these shapes inflates its own measure); user32.h has 17 CONST:: lines, 4 bare ones at
    //user32.h:243, user32.h:570, user32.h:626 and user32.h:627, and never writes CONST ::. CONST is an SDK macro -
    //minwindef.h:153 under its own #ifndef, and ntdef.h:227 - so the three forms reach the compiler as
    //const::RECT*, const ::RECT* and const RECT*: 31, 16 and 49 occurrences in tmp/_tmp_rect_pp_pre_s28.txt.
    //
    //Return conventions (U1/U2 - recorded, not changed): no Win32ErrorHandler and no CheckReturn anywhere here, and
    //none of the eleven SDK prototypes carries _Check_return_ (over winuser.h:9749-:9827 the only qualifiers are
    //WINUSERAPI / BOOL / WINAPI), so a discarded FALSE is silent at the SDK side too. What this section adds to the
    //item is that the eleven BOOLs mean three different things. EqualRect, IsRectEmpty and PtInRect return an
    //answer, so BoolCheck would report a legitimate FALSE as an error (session 27's WaitForInputIdle inverse case,
    //now four instances). IntersectRect, UnionRect and SubtractRect return zero for a documented outcome rather than
    //a failure - IntersectRect puts an all-zero rectangle in lprcDst when the sources do not intersect, SubtractRect
    //returns zero when the result is empty, UnionRect when no nonempty rectangle results. Only CopyRect,
    //InflateRect, OffsetRect, SetRect and SetRectEmpty are documented as success-or-failure, and all five discard
    //FALSE with no trace; U7 keeps a swallowed SEH fault indistinguishable from a legitimate zero.
    //Version guards (U16): 0 of the eleven prototypes carry a version guard of any kind. The innermost guards over
    //the whole block are #if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) at winuser.h:9717, the file-wide
    //#ifndef NOUSER at :64 and #ifndef _WINUSER_ at :13, so all eleven are desktop-family only but never
    //version-gated (tmp/_tmp_sdk_guard_scan.ps1, run over these stems by tmp/_tmp_rect_pre_s28.ps1, output
    //tmp/_tmp_rect_pre_s28.txt). NOUSER cannot be defined for this TU in any case - RECT, LPRECT and POINT come from
    //those same headers.
    //
    //Reachability (D14) and U19: each of the eleven has exactly one layer caller - the qor::platform::win::Rect
    //facade in ui/gui/view/drawing/rect.cpp (:37 CopyFrom, :42 Equal, :47 Inflate, :53 FromIntersection, :59
    //IsEmpty, :64 Offset, :70 IsPointInRect, :75 Set, :80 SetEmpty, :86 FromSubtraction, :93 FromUnion). Over the 44
    //spellings of the eleven stems the scan gives 11 LAYER / 0 real INDIRECT / 3 BYPASS / 0 MEMBER
    //(tmp/_tmp_rect_bypass_s28.txt). Its two INDIRECT rows are a false-positive class not recorded before:
    //ui/gui/controllers/a_h/editcontroller.cpp:220 and ui/gui/gdiobjects/region.cpp:48 are same-named member
    //DEFINITIONS - Region::SetRect calls GDI32::SetRectRgn - not calls into user32.dll. The three BYPASS sites are
    //live code in a compiled target (ui/gl/CMakeLists.txt:5): ui/gl/windowimpl.cpp:516 and :521 (an unqualified
    //OffsetRect beside a WindowController::AdjustRect* call) and :1756 (::SetRect). A fourth OffsetRect there, at
    //windowimpl.cpp:1470, sits inside the block comment that opens at :1303 and closes at :1507, so it is not a
    //call site at all (tmp/_tmp_comment_state.ps1). Three of the eleven layer sites drop the BOOL outright:
    //rect.cpp:53, :86 and :93 hand back a Rect the SDK filled, so one level above the layer "the sources do not
    //intersect" is indistinguishable from "the result is empty".
    //
    //Coverage (method step 7): this section's eleven exports are all wrapped. Sharing the SDK sub-block,
    //DrawFocusRect (winuser.h:9722) is wrapped in painting.cpp, FillRect (winuser.h:9729), FrameRect
    //(winuser.h:9737) and InvertRect (winuser.h:9745) in filledshapes.cpp, SetSysColors (winuser.h:9706) is declared
    //at user32.h:651 as a member no TU defines (U4's population), and GetWindowWord (ordinal 1FB / 00086890,
    //winuser.h:9837) and SetWindowWord (388 / 0002FD20, winuser.h:9844) have no member anywhere in user32.h. The
    //family measure (tmp/_tmp_rect_family_s28.ps1) over the 51 user32 export names containing "Rect": 41 declared in
    //winuser.h, 39 with a member, so the SDK-visible gaps are those two WindowWord names plus
    //EvaluateProximityToRect and GetPointerDeviceRects from the pointer family. No header line was touched.
    //
    //Call-site idiom: 11/11 qor_pp_fcontext and 11/11 explicit Call<> template arguments (0 argument-deducing; the
    //folder's 3 deducing sites are all in window.cpp). All eleven sites now use the folder's tight Call< spelling -
    //this TU was 11 spaced / 0 tight, and normalising it moves folder U14(a) from 174 spaced / 254 tight to
    //163 / 265 by user decision. Its ../returncheck.h include (then :11) used nothing from it: one of T7's 35 dead
    //includes, removed 5 Nov 2026 when the folder-wide decision behind T7 was taken.

    BOOL User32::CopyRect(::LPRECT lprcDst, const::RECT* lprcSrc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, CopyRect);
        return Library::Call<BOOL, ::LPRECT, const ::RECT*>(pFunc, lprcDst, lprcSrc);
    }

    BOOL User32::EqualRect(const::RECT* lprc1, const::RECT* lprc2)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EqualRect);
        return Library::Call<BOOL, const ::RECT*, const RECT*>(pFunc, lprc1, lprc2);
    }

    BOOL User32::InflateRect(::LPRECT lprc, int dx, int dy)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, InflateRect);
        return Library::Call<BOOL, ::LPRECT, int, int>(pFunc, lprc, dx, dy);
    }

    BOOL User32::IntersectRect(::LPRECT lprcDst, const::RECT* lprcSrc1, const::RECT* lprcSrc2)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IntersectRect);
        return Library::Call<BOOL, ::LPRECT, const ::RECT*, const ::RECT*>(pFunc, lprcDst, lprcSrc1, lprcSrc2);
    }

    BOOL User32::IsRectEmpty(const::RECT* lprc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, IsRectEmpty);
        return Library::Call<BOOL, const ::RECT*>(pFunc, lprc);
    }

    BOOL User32::OffsetRect(::LPRECT lprc, int dx, int dy)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, OffsetRect);
        return Library::Call<BOOL, ::LPRECT, int, int>(pFunc, lprc, dx, dy);
    }

    BOOL User32::PtInRect(const::RECT* lprc, POINT pt)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, PtInRect);
        return Library::Call<BOOL, const ::RECT*, POINT>(pFunc, lprc, pt);
    }

    BOOL User32::SetRect(::LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetRect);
        return Library::Call<BOOL, ::LPRECT, int, int, int, int>(pFunc, lprc, xLeft, yTop, xRight, yBottom);
    }

    BOOL User32::SetRectEmpty(::LPRECT lprc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SetRectEmpty);
        return Library::Call<BOOL, ::LPRECT>(pFunc, lprc);
    }

    BOOL User32::SubtractRect(::LPRECT lprcDst, const::RECT* lprcSrc1, const::RECT* lprcSrc2)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, SubtractRect);
        return Library::Call<BOOL, ::LPRECT, const ::RECT*, const ::RECT*>(pFunc, lprcDst, lprcSrc1, lprcSrc2);
    }

    BOOL User32::UnionRect(::LPRECT lprcDst, const::RECT* lprcSrc1, const::RECT* lprcSrc2)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UnionRect);
        return Library::Call<BOOL, ::LPRECT, const ::RECT*, const RECT*>(pFunc, lprcDst, lprcSrc1, lprcSrc2);
    }

}}}//qor::win::api
