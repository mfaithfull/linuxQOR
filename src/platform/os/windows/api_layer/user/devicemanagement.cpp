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

    //The "Device Management functions" section (user32.h:681-683): two declarations, two definitions here, in the
    //header's order, both static (U13 clean), both qualified User32:: and both call sites stating the full SDK
    //argument list (U14: 2/2 explicit). RegisterDeviceNotification has an SDK A/W family (winuser.h:3952-3958 A,
    //:3959-3965 W, with the stem -> W macro at :3966-3970), so the wrapper spells the binding
    //qor_pp_useswinapiAW; UnregisterDeviceNotification (winuser.h:3972-3977) is a single export with no A/W
    //variants, so the bare qor_pp_useswinapi is its D13 spelling. cl /EP on this TU resolves the two literals
    //"RegisterDeviceNotificationW" and "UnregisterDeviceNotification" (tmp/_tmp_devman_pp.txt:453294, :453301 - the line
    //numbers of the 30 Oct run; they shift with the size of this comment block, the literals do not);
    //the export table has RegisterDeviceNotificationA (2E4), RegisterDeviceNotificationW (2E5) and
    //UnregisterDeviceNotification (3C6), and the live probe (tmp/_tmp_name_resolve_probe.cpp) adds the two facts
    //the table alone cannot give: bare "RegisterDeviceNotification" is MISSING - this stem is one of the nine
    //user32.h:9-17 #undef's (G2), so a bare binding here would throw "Missing library function." at run time -
    //while "UnregisterDeviceNotificationA"/"W" are MISSING too, which is what makes the bare spelling correct.
    //RegisterDeviceNotificationA is exported but unreachable through this surface. The wrong-stem scan reports no
    //binding-vs-member disagreement in this file.
    //This member is one of the 11 U17 deviations (an A/W family with no T suffix) and the only one whose stem is
    //also #undef'd, but the AW macro token-pastes the W before the SDK macro could be consulted, so the #undef
    //never reaches the binding - the cost of the missing suffix is purely readability, and it is sharper here than
    //elsewhere because the A/W choice is invisible in the signature: the filter is a bare LPVOID, and the W entry
    //point reads the payload's trailing name field (DEV_BROADCAST_DEVICEINTERFACE.dbcc_name /
    //DEV_BROADCAST_HANDLE.dbh_name, both TCHAR) as wide. Which half is called is therefore an unstated contract on
    //the caller's struct, not something the wrapper type says. Renaming is deferred to the folder-wide U17 pass
    //(session-11 policy), so no name action here.
    //Return conventions agree in this section: RegisterDeviceNotification returns NULL on failure (HDEVNOTIFY is
    //typedef PVOID at winuser.h:3943-3944) and UnregisterDeviceNotification returns FALSE on failure, so both zero
    //values mean failure and neither is checked (U1/U2 settled policy: no Win32ErrorHandler, no CheckReturn).
    //Unlike the ChangeDisplaySettings pair, the SEH swallow (U7) cannot lie here - Call's default ret _{} is
    //NULL/FALSE, which is exactly what a documented failure returns - so a fault is silent, but not misleading.
    //Handle contract: the HDEVNOTIFY the first returns is released only by the second (never Kernel32::CloseHandle),
    //it must be unregistered before the recipient goes away or the registration leaks, and passing NULL to the
    //second returns FALSE rather than faulting.
    //Parameter contracts: hRecipient is an HWND or a service status handle depending on Flags, and the accepted
    //Flags values are DEVICE_NOTIFY_WINDOW_HANDLE / DEVICE_NOTIFY_SERVICE_HANDLE (winuser.h:3946-3947) plus
    //DEVICE_NOTIFY_ALL_INTERFACE_CLASSES (0x4) under _WIN32_WINNT >= 0x0501 (:3948-3950). NotificationFilter must
    //point at a device-structure whose dbch_size and dbch_devicetype the caller has already filled in - the wrapper
    //passes the LPVOID straight through and cannot check either. Neither those Flags constants nor the
    //DEV_BROADCAST_* payload types are re-exported by this library; the only QOR mirrors are winqor_common's
    //DevBroadcastDeviceInterface / DevBroadcastHeader (src/platform/os/windows/common/structures.h:399-413), which
    //this module does not link, so a caller needs either the SDK headers or that sibling module to call correctly.
    //Section coverage: the SDK's device-notification sub-block (winuser.h:3942-3977, inside the Desktop region that opens at
    //:3939-3940) is fully covered, 2 of 2 entry points. The block that follows it inside the same guards - the Power
    //Management pair RegisterPowerSettingNotification (2F1) and UnregisterPowerSettingNotification (3CB), winuser.h:3993,
    //:4002 under #if (_WIN32_WINNT >= 0x0502) (:3979-:4022) - is declared at user32.h:686-687 but defined in no TU in this
    //folder, and has no owning file in the folder at all; those two declarations are also not static, unlike this
    //pair, so they are U4 members and a caller gets LNK2019 (deferred task T3 in status.md covers that, along with
    //RegisterSuspendResumeNotification 2F6 / UnregisterSuspendResumeNotification 3CD at winuser.h:4009/:4017, which this
    //header does not list at all). U16 exposure: both wrappers' prototypes sit inside #if(WINVER >= 0x0500)
    //(:3942) and #if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) (:3940), and HDEVNOTIFY itself is only
    //typedef'd at :3943 under the same guards - so a WINVER < 0x0500 or non-Desktop consumer does not merely lose
    //the prototypes, user32.h:682-683 becomes ill-formed on an unknown type. No WINVER is pinned in cmake/ or in
    //any CMakeLists.txt (measured this session), so sdkddkver.h's default applies.
    //Liveness: neither wrapper has any call site in src/, examples/ or test/ (2/2 U15 instances), and the family has
    //one direct-SDK bypass site, ui/gl/helperwindow.cpp:85 (::RegisterDeviceNotificationW, live code, and that call
    //does fill dbcc_size/dbcc_devicetype/dbcc_classguid first, :79-83). That file is in no source list - winqor_gl
    //is winqor_gl.cpp cursorimpl.cpp windowimpl.cpp wgl.cpp - and ui/CMakeLists.txt:4 comments add_subdirectory(gl)
    //out of the build, so the bypass is source debt rather than run-time behaviour (U19: 0 LAYER / 1 BYPASS).

    HDEVNOTIFY User32::RegisterDeviceNotification(HANDLE hRecipient, LPVOID NotificationFilter, DWORD Flags)
    {
        qor_pp_fcontext;
        qor_pp_useswinapiAW(user32, RegisterDeviceNotification);
        return Library::Call< HDEVNOTIFY, HANDLE, LPVOID, DWORD >(pFunc, hRecipient, NotificationFilter, Flags);
    }

    BOOL User32::UnregisterDeviceNotification(HDEVNOTIFY Handle)
    {
        qor_pp_fcontext;
        qor_pp_useswinapi(user32, UnregisterDeviceNotification);
        return Library::Call< BOOL, HDEVNOTIFY >(pFunc, Handle);
    }

}}}//qor::win::api
