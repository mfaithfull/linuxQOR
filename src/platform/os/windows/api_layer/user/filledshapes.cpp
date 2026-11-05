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

    //The "Filled Shapes functions" section (user32.h:608-611): three declarations, three definitions here, in the
    //header's order, all static, all qualified User32::, all three call sites explicit about the SDK argument types
    //(U14: 3/3 explicit, 3/3 spaced style). The section is fully covered by this TU - no U4 member, no U11
    //unqualified definition, no U13 non-static member.
    //All three entry points are single exports with no A/W family, so the bare qor_pp_useswinapi is the D13 spelling
    //and the AW form would break all three. cl /EP on this TU resolves the literals "FillRect", "FrameRect",
    //"InvertRect" (tmp/_tmp_fs_pp.txt:453304, :453311, :453318 of the 31 Oct post-edit run - cl /EP keeps one output
    //source line, so those numbers shift with the weight of this comment block while the literals do not); the export
    //table gives FillRect 110, FrameRect 117, InvertRect 226, and the live probe (tmp/_tmp_name_resolve_probe.cpp)
    //reports all three RESOLVED with every *A and *W spelling MISSING. None of the three stems is #undef'd at
    //user32.h:9-17 and none has a stem -> stemW macro in the SDK (tmp/_tmp_winuser_aw_macros.txt has no hit for any
    //of them), so G1 has nothing to expand and the spelling in the source is the name handed to GetProcAddress. The
    //wrong-stem scan (tmp/_tmp_wrong_stem_scan.ps1) reports no binding-vs-member disagreement in this file.
    //CONST::RECT* is the SDK's CONST macro glued to the global-namespace qualifier, not a namespace: CONST is defined
    //as const at minwindef.h:153 (and ntdef.h:227), so the parameter preprocesses to const ::RECT* - proved on this TU
    //at tmp/_tmp_fs_pp.txt:453301 ("int User32::FillRect(HDC hDC, const::RECT* lprc, HBRUSH hbr)"). It compiles and
    //means what it points at, and the same spelling carries the 11 Rectangle members (user32.h:447-457,
    //rectangle.cpp) and the Painting ones (user32.h:264-270, painting.cpp), so it is a folder-wide style item, not
    //something to rename inside one TU. It does make tmp/_tmp_sigcmp.ps1 print a false "CALL TYPE MISMATCH" on all
    //three sites here (decl type 'CONST::RECT*' vs template argument 'CONST ::RECT*') - the instrument compares
    //spelling, and the types are identical after preprocessing.
    //Return conventions, all three matching the SDK: FillRect int (winuser.h:9726-9732) and FrameRect int
    //(:9734-9740), each 0 on failure; InvertRect BOOL (:9742-9747), FALSE on failure. No U18 instance. Per the settled
    //policy this TU adds no Win32ErrorHandler and no CheckReturn (U1/U2: 3 call sites, 0 handlers, 0 checks), so a
    //0/FALSE failure is silent, and per U7 an SEH fault inside the callee is indistinguishable from a legitimate zero.
    //The only in-tree reader of any result folds InvertRect's BOOL into bool
    //(ui/gui/gdiobjects/devicecontext.cpp:85); the two int results are passed through and read nowhere.
    //Contracts passed through unchanged: hDC must be a live DC and lprc is in that DC's client coordinates for a
    //window DC. FillRect paints the whole rect with the brush's style and neither uses nor moves the brush origin;
    //FrameRect paints only the border with the brush's frame style, so for a pattern brush it degenerates to
    //FillRect; InvertRect inverts the bits inside the rect with the DC's current brush and does not draw the border -
    //FrameRect does that. None of the three selects a brush, so there is no DC state to restore afterwards, and the
    //HBRUSH is borrowed: the caller keeps ownership and must keep the brush alive across the call.
    //U16 guard stack, measured on winuser.h 10.0.26100 with tmp/_tmp_sdk_guard_scan.ps1: the three prototypes (:9729,
    //:9737, :9745) sit open only under #if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) (:9717) inside
    //#ifndef NOUSER (:64) - no WINVER/_WIN32_WINDOWS guard at all, so this TU adds 0 of 3 to U16. Nor is there a
    //type-level sub-case: RECT is declared at shared/windef.h:154-160 under the Application/Games/System partition
    //(:152), i.e. outside the Desktop guard that wraps these prototypes, so a non-Desktop partition would lose the
    //prototypes while user32.h:609-611 still compiles - the wrapper binds by name and never calls the SDK prototype.
    //Section coverage: the SDK's own block is the contiguous Desktop-Family region winuser.h:9716-9852 - 15
    //shape/rect entry points (DrawFocusRect, FillRect, FrameRect, InvertRect, SetRect, SetRectEmpty, CopyRect,
    //InflateRect, IntersectRect, UnionRect, SubtractRect, OffsetRect, IsRectEmpty, EqualRect, PtInRect) plus
    //GetWindowWord/SetWindowWord inside #ifndef NOWINOFFSETS (:9832-9847). This library declares 15 of them but split
    //three ways: the 3 here, the 11 rect helpers at user32.h:447-457 (rectangle.cpp) and DrawFocusRect at
    //user32.h:267 (painting.cpp:44) - U21's split shape again, and this section is complete on its own terms.
    //Declared nowhere in the header: AlignRects 6 and EvaluateProximityToRect 10D (both exported, both RESOLVED in the
    //probe) and the NOWINOFFSETS pair, which belongs to windowproperties.cpp' section.
    //Liveness (U15): each wrapper has exactly one call site, all three in DeviceContext::FillRect/FrameRect/InvertRect
    //(ui/gui/gdiobjects/devicecontext.cpp:75, :80, :85, built at gui/CMakeLists.txt:44). Those three DeviceContext
    //methods are non-virtual (devicecontext.h:232-234) and have no call site anywhere in src/examples/test, so the
    //chain is compiled, exported and never executed. Unlike most such chains the DC it would run on is live:
    //WindowController::GetDC feeds mainwindow.cpp:84 and PaintSession's DC feeds mainwindow.cpp:173, whose OnPaint
    //calls dc->GetTextExtentPoint32T/TextOutT (:179, :180) on a DeviceContext and a Rect - it just never calls these
    //three. The U19 scan over the three stems gives 3 LAYER / 3 INDIRECT / 0 BYPASS / 0 MEMBER
    //(tmp/_tmp_fs_bypass.txt): nothing in the tree reaches these entry points through the SDK import either.
    //Caller-side cast checked rather than assumed: devicecontext.cpp:75/:80/:85 pass reinterpret_cast<const RECT*>(&rc)
    //where rc is qor::platform::win::Rect (ui/gui/view/drawing/rect.h:33-38 - long m_left, m_top, m_right, m_bottom)
    //against RECT (LONG left, top, right, bottom at shared/windef.h:154-160): field-for-field the same order and
    //widths on this toolchain, so the alias is sound in practice - the same hand-mirror pattern as the
    //COMBOBOXINFO/MSGBOXPARAMS sites noted under U15/U23. It remains a reinterpret_cast between unrelated types rather
    //than a conversion, which is a winqor_gui observation, not a defect in this layer.

    int User32::FillRect(HDC hDC, CONST::RECT* lprc, HBRUSH hbr)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, FillRect);
        return Library::Call< int, HDC, CONST ::RECT*, HBRUSH >(pFunc, hDC, lprc, hbr);
    }

    int User32::FrameRect(HDC hDC, CONST::RECT* lprc, HBRUSH hbr)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, FrameRect);
        return Library::Call< int, HDC, CONST ::RECT*, HBRUSH >(pFunc, hDC, lprc, hbr);
    }

    BOOL User32::InvertRect(HDC hDC, CONST::RECT* lprc)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, InvertRect);
        return Library::Call< BOOL, HDC, CONST ::RECT* >(pFunc, hDC, lprc);
    }

}}}//qor::win::api
