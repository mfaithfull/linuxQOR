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
    //Five wrappers - the whole "user32.h:587 //Monitor functions" section, declared :588-592 and defined here in header
    //order (:127 EnumDisplayMonitors .. :155 MonitorFromWindow). All five declarations are static and all five are defined
    //here through the User32:: qualifier, so 0 U4 (declared, never defined), 0 U11 (definition unqualified) and 0 U13
    //(defined but declared non-static) instances, and the declaration-vs-definition signature compare over the section
    //gives 0 flags (tmp/_tmp_mon_pre_s24.txt).
    //
    //Binding (D13): four of the five stems are genuine single exports and bind bare - EnumDisplayMonitors (ordinal FE,
    //RVA 0002ED90), MonitorFromPoint (28E), MonitorFromRect (28F), MonitorFromWindow (290) in
    //tmp/_tmp_user32_exports.txt - while GetMonitorInfo is an A/W family (18C/18D) with no bare export at all, so the
    //qor_pp_useswinapiAW at :137 is load-bearing rather than stylistic: a bare binding there would ask GetProcAddress
    //for a name the DLL does not have and pFunc would cache NULL for the process lifetime (G5, U5).
    //tmp/_tmp_mon_class_s24.ps1 checks all five three ways (export-table presence of bare/<stem>A/<stem>W, and whether
    //winuser.h defines a macro for the stem at all): only GetMonitorInfo has one (winuser.h:13696-13700), and no stem is
    //P6-shaped (a bare export that is really the A entry point while a W sibling exists), so 0 P2 and 0 P6 here. This TU
    //contributes 0 of the folder's 11 unresolvable bindings (tmp/_tmp_p1census_pre_s24.txt, 428 bindings scanned).
    //cl /EP over the TU emits exactly five literals - "EnumDisplayMonitors" (:453254), "GetMonitorInfoW" (:453261),
    //"MonitorFromPoint" (:453268), "MonitorFromRect" (:453275), "MonitorFromWindow" (:453282 of
    //tmp/_tmp_mon_pp_pre_s24.txt) - and a live GetProcAddress probe resolves all five at the export-table RVAs while
    //reporting MISSING for bare GetMonitorInfo, for every *T spelling and for GetMonitorInfoExW/A, which do not exist as
    //exports (tmp/_tmp_mon_probe_s24.cpp + tmp/_tmp_run_mon_probe_s24.bat -> tmp/_tmp_mon_probe_s24.txt). The probe had
    //to LoadLibraryW first: GetModuleHandleW(L"user32") fails with gle=126 in a bare console process, which is G5. The
    //wrong-stem scan has no monitor.cpp row - the five bindings all name the member that encloses them
    //(tmp/_tmp_wrongstem_s24.txt, 4 mismatches in the folder, none here).
    //
    //Signatures: every parameter and return type pairs field-for-field with the SDK prototype - MonitorFromPoint
    //winuser.h:13607-13612, MonitorFromRect :13614-13619, MonitorFromWindow :13621-13626, GetMonitorInfoA :13684-13689,
    //GetMonitorInfoW :13690-13695, EnumDisplayMonitors :13704-13711 - and the parameter names match the SDK's too, so
    //0 U18 divergences in either half. MonitorFromPoint takes POINT by value exactly as the SDK does; since the U6 fix
    //Library::Call builds its fPtr from the very template arguments the call site spells and qualifies it __stdcall
    //(library.h:43-63), so both the by-value struct and the MONITORENUMPROC callback travel through a pointer type that
    //matches the export on x64 and x86. All five Call<> lists are explicit (U14b) and each restates its own signature,
    //so this TU adds no U14 instance - unlike mdi.cpp, where the list contradicted the member.
    //
    //Availability (U16): all six paired SDK declarations sit under one version guard - #if(WINVER >= 0x0500) at
    //winuser.h:13594, which opens the whole multimonitor block - plus the Desktop partition (#if
    //WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) at :13605 and :13638). This is the first TU of the review whose
    //entire section shares a single guard: a consumer that pins WINVER below 0x0500 loses all 6 prototypes at once while
    //this library's 5 declarations and the 6 exports stay, so the section becomes wholly uncallable through the header.
    //Nothing in cmake/, the root CMakeLists.txt, CMakeSettings.json or src defines WINVER, _WIN32_WINNT or
    //NTDDI_VERSION (measured: 0 hits), so the SDK default applies and no pin is written here - session 5's deferral
    //stands (tmp/_tmp_mon_guards_s24.ps1 -> tmp/_tmp_mon_guards_s24.txt, 6 hits, 6 guarded).
    //
    //Section against SDK (U21, sixth instance and the largest region so far): winuser.h has no NOMONITOR macro at all,
    //so this family has no opt-out marker of its own, and the guard at :13594 owns the region up to :14905 - 45 distinct
    //declaration heads, of which only the 6 monitor ones are wrapped in this section. The other 39 share the region:
    //the DPI contexts cluster, the GetWindowInfo/GetComboBoxInfo/GetCursorInfo/GetMenuBarInfo/GetScrollBarInfo/
    //GetTitleBarInfo neighbourhood, the WinEvent-hook pair SetWinEventHook/UnhookWinEvent with NotifyWinEvent and
    //IsWinEventHookInstalled, GetAltTabInfoA/W, RealGetWindowClassA/W, GetWindowModuleFileNameA/W, GetAncestor,
    //BlockInput, GetDpiForWindow, GetGUIThreadInfo, RealChildWindowFromPoint, IsProcessDPIAware, SetProcessDPIAware and
    //InheritWindowMonitor (full list: tmp/_tmp_mon_pre2_s24.txt section (b)). tmp/_tmp_mon_region_s24.ps1 ->
    //tmp/_tmp_mon_region_s24.txt then asks who wraps them: 21 of the 45 heads have a User32:: definition in this folder
    //and 24 do not - 2 of those 24 are instrument artifacts, because this TU covers GetMonitorInfoA and GetMonitorInfoW
    //with its single GetMonitorInfoT wrapper. The honest split is therefore 6 heads here, 17 defined elsewhere (window.cpp
    //8, accessibility.cpp 4, and one each in keyboard.cpp, combobox.cpp, cursor.cpp, menu.cpp and scrollbar.cpp) and 22
    //with no wrapper anywhere. The 23 non-monitor declarations sit in nine header sections - Keyboard Input :72, Window
    //functions :157, :161, :167, :170, :182, :192, :199, Menu :226, Cursor :388, List Box :548, Combo Box :553,
    //ScrollBar :575, Active Accessibility :662-:665, plus GetDpiForWindow :639, which has no section label of its own and
    //merely follows the "WINNLSEnableIME is obsolete - do not use" note - so one SDK region feeds ten sections and eight
    //TUs, and no TU's own view of the WINVER 0x0500 surface exceeds 8 of its 45 heads. The label and the contents of this
    //section do agree, so the keyboard.cpp and devicecontext.cpp mirror case does not arise - the split is one region
    //feeding several sections, not a mislabelled section. One hand-off falls out of the scan: GetListBoxInfo, declared at
    //user32.h:548 in the "//List Box functions" section, is a live export (ordinal 174, RVA 0002EFF0) with a declaration
    //and no definition in any TU and no call site anywhere in src, examples or test - a U4 instance for the listbox
    //session, recorded here so it is not lost.
    //
    //Structs (U26 fourth instance, and the safe half of the hazard): user32.h:588-592 name no TCHAR-bearing type, so
    //unlike GetIconInfoExT this section cannot propagate the include-order width through its own surface - GetMonitorInfoT
    //publishes ::LPMONITORINFO, the 40-byte MONITORINFO, and neither MONITORINFOEX nor LPMONITORINFOEX appears in the
    //header. The A/W choice is nevertheless real and it is invisible in the signature: GetMonitorInfo is one contract
    //with two struct shapes chosen by cbSize (winuser.h:13640-13682), there is no GetMonitorInfoEx* export, and the
    //binding here is unconditionally the W one. Measured on x64 and x86 (tmp/_tmp_mon_mirror_s24.cpp +
    //tmp/_tmp_mon_mirror_s24.bat): the caller-side mirror MonitorInfo at ui/gui/controllers/i_m/monitor.h:47-53 is
    //layout-identical to MONITORINFO on both targets (40 bytes; cbSize@0, rcMonitor@4, rcWork@20, flags@36) because it
    //spells its fields unsigned long/long rather than portable 64-bit types, and MonitorInfoEx (:55-58) is 104 bytes =
    //MONITORINFOEXW exactly (szDevice@40, 32 wchar_t, DeviceNameSize = 32 at common/constants.h:72) because QOR spells
    //TCHAR from qor_pp_unicode (common/structures.h:58, src/configuration/configuration.h:13 = 1) rather than from
    //UNICODE. The mirror and the pasted W binding agree only while that one switch holds: the same mirror compiled in
    //the narrow world is 72 bytes = MONITORINFOEXA against an export that fills the W shape, which is the
    //icon.cpp GetIconInfoExT overrun shape with the roles reversed. Point and Rect (view/drawing/point.h:31-35,
    //view/drawing/rect.h:33-39) are byte-identical to POINT and RECT on both arches (8 and 16 bytes), so the by-value
    //POINT at :145 below and the caller's reinterpret_casts at i_m/monitor.cpp:236/:242 are sound - unlike the
    //Message/MSG mirror, which diverges on x86. The one genuine x86 divergence in this section is the callback: the
    //caller's MonitorEnumProc (monitor.h:38) takes a long long where the SDK's MONITORENUMPROC takes LPARAM, 8 bytes
    //against 4 on x86. x64 matches and only x64-Debug is configured, so this is recorded against U26 and left to
    //winqor_gui.
    //
    //Contracts the wrappers do not enforce: GetMonitorInfoT needs the caller to have set cbSize before the call and
    //fails with ERROR_INVALID_PARAMETER otherwise, and nothing here can see that - the published ::LPMONITORINFO cannot
    //express the MONITORINFOEX shape at all, which is why the only in-tree caller reaches the wrapper through
    //reinterpret_cast<::LPMONITORINFO>(&info) at ui/gui/controllers/i_m/monitor.cpp:56 and :62, setting cbSize
    //explicitly at :61 for the Ex overload and relying on the mirror's default member initialiser (monitor.h:49) for
    //the plain one. EnumDisplayMonitors' lpfnEnum is _In_, so a NULL callback is not valid (hdc and lprcClip are
    //_In_opt_, so NULL/NULL is a valid pair); the parameter type ::MONITORENUMPROC, the SDK's own typedef at
    //winuser.h:13702, carries the CALLBACK/__stdcall convention, but nothing enforces the FALSE-means-stop-return
    //contract on the way back. All five returns indicate failure - FALSE for the first two, NULL for the three
    //MonitorFrom* calls, and only under MONITOR_DEFAULTTONULL - and there is no CheckReturn and no Win32ErrorHandler in
    //this TU per the standing U1/U2 policy, so a swallowed SEH fault is indistinguishable from a legitimate zero (U7).
    //Every type here is spelled the way the SDK spells it, so STRICT raises no U27 identity question for this section.
    //
    //The call path (U19) and the first bypass found in BUILT code: 8 LAYER / 0 INDIRECT / 4 BYPASS / 0 MEMBER over the
    //8 spellings of the five stems (tmp/_tmp_mon_bypass_s24.ps1 -> tmp/_tmp_mon_bypass_s24.txt, comment-stripped), so
    //all five wrappers have at least one through-layer site: GetMonitorInfoT at ui/gui/controllers/i_m/monitor.cpp:56
    //and :62, EnumDisplayMonitors at :187 and :228, MonitorFromPoint :236, MonitorFromRect :242, MonitorFromWindow :248
    //and windowcontroller.cpp:679 - and unlike every earlier TU all eight LAYER rows are in code winqor_gui builds
    //(ui/gui/CMakeLists.txt:25 and :27, reached from ui/CMakeLists.txt:5). Three of the four bypass rows are
    //GetMonitorInfoW import thunks in ui/gl/monitorimpl.cpp:79 and ui/gl/windowimpl.cpp:435 and :1902: live code once
    //comments are stripped, but winqor_gl is commented out at ui/CMakeLists.txt:4 and monitorimpl.cpp is in no source
    //list at all (ui/gl/CMakeLists.txt compiles only winqor_gl.cpp, cursorimpl.cpp, windowimpl.cpp and wgl.cpp). The
    //fourth row is the new fact: i_m/monitor.cpp:163 calls the GetMonitorInfoW import from the very translation unit
    //that calls User32::GetMonitorInfoT at :56 and :62, so one export is reached through both paths in one file and the
    //bypass runs. Recorded, not fixed - collapsing it is a winqor_gui change, not a change to this layer.

    BOOL User32::EnumDisplayMonitors(HDC hdc, LPCRECT lprcClip, ::MONITORENUMPROC lpfnEnum, LPARAM dwData)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, EnumDisplayMonitors);
        return Library::Call< BOOL, HDC, LPCRECT, ::MONITORENUMPROC, LPARAM>(pFunc, hdc, lprcClip, lpfnEnum, dwData);
    }

    BOOL User32::GetMonitorInfoT(HMONITOR hMonitor, ::LPMONITORINFO lpmi)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, GetMonitorInfo);
        return Library::Call<BOOL, HMONITOR, ::LPMONITORINFO>(pFunc, hMonitor, lpmi);
    }

    HMONITOR User32::MonitorFromPoint(::POINT pt, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, MonitorFromPoint);
        return Library::Call<HMONITOR, ::POINT, DWORD>(pFunc, pt, dwFlags);
    }

    HMONITOR User32::MonitorFromRect(LPCRECT lprc, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, MonitorFromRect);
        return Library::Call<HMONITOR, LPCRECT, DWORD>(pFunc, lprc, dwFlags);
    }

    HMONITOR User32::MonitorFromWindow(HWND hwnd, DWORD dwFlags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, MonitorFromWindow);
        return Library::Call<HMONITOR, HWND, DWORD>(pFunc, hwnd, dwFlags);
    }

}}}//qor::win::api
