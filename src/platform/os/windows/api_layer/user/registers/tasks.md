# T1-T4 deferred tasks, each needs a decision (verbatim)

### Deferred tasks (opened by session 10; T3 added by session 12) - each needs a decision, so none was touched

- **T1 - two declared-but-undefined members in the "Authorization functions" section.** `user32.h:658-659` declare
  `GetUserObjectSecurity` and `SetUserObjectSecurity` **without `static`** and **no `.cpp` in the folder defines either**
  (measured: the only hits outside the header are the `desktop.cpp` TU comment), so any caller of either fails at link
  time with `LNK2019` rather than at compile time - the same U4 shape as the other 95 non-static declarations without a
  definition. What sets this pair apart is that their entry points are real, exported and single-stem: `GetUserObjectSecurity`
  is export ordinal 1DB (hint 1979, RVA 00002460) and `SetUserObjectSecurity` ordinal 375 (hint 2384, RVA 0002BBE0) in
  `tmp/_tmp_user32_exports.txt`, and the SDK
  declares both at `winuser.h:1671-1684`, directly after `SetUserObjectInformation`/`GetUserObjectInformation`, which
  `desktop.cpp:153` and `:118` wrap. Two callers-away neighbours: `UserHandleGrantAccess` is the third user-object call
  and *is* defined - but in another TU's section (`user32.h:259` "Process and Thread functions", defined
  `processthread.cpp:41`), a U12-style placement fact for that session. Deciding T1 means either adding the two
  definitions (natural home: `desktop.cpp`, which owns the rest of the user-object family) or deleting the declarations -
  both move the published surface, so it is a user decision and belongs with the `U4`/`U13` sweep, not with a TU review.
- **T2 - the whole Window Station / Desktop surface is dead exported API, and its conventions are therefore untestable
  against a caller.** All 18 `desktop.cpp` wrappers have **zero** in-tree callers (searched `src`, `examples`, `test`
  for `User32::<name>` across all 18 - only the definitions match), and a comment-stripped search for direct SDK calls
  (`OpenWindowStation|OpenDesktop|SwitchDesktop|GetUserObjectInformation(W|A)?\(` excluding `User32::`) finds **zero
  bypass sites** - the single hit is the declaration at `user32.h:416`. So this section is 18/18 U15 instances and
  0/18 U19 sites, which makes several folder decisions (U17 `T`-suffix consistency, P3, and whether `EnumDesktopsT`'s
  callback should be typed as the SDK's `DESKTOPENUMPROC` or as a `LPCWSTR`-taking callback now that the binding is
  always the `W` form) pure taste calls with no call site to inform them. Recorded for the folder-wide pass; nothing in
  this section is exercised by `examples/mainwindow`, so no session can observe a behaviour change here.
- **T3 - the "Power Management functions" pair is declared but has no owning TU at all.** Opened 30 Oct 2026 (session 12)
  while mapping the section that follows "Device Management functions": `user32.h:685` `//Power Management functions`
  declares `RegisterPowerSettingNotification` (`:686`) and `UnregisterPowerSettingNotification` (`:687`) - **neither is
  `static`**, unlike the device pair just above and unlike 427 of the header's 525 members - and **no `.cpp` in the folder
  defines either**; there is no `powermanagement.cpp` (nor any other file) that could hold them: the folder's 38 sources
  contain no match for either name outside the header. So they are U4 members of the plainest kind (a caller gets
  `LNK2019`, not `C2352`), and this is the sharpest U4 shape in the folder because both entry points are real and exported -
  `RegisterPowerSettingNotification` ordinal 2F1 and `UnregisterPowerSettingNotification` ordinal 3CB, both **RESOLVED** by
  the live probe (`tmp/_tmp_name_resolve_probe.cpp`) - and the SDK declares both at `winuser.h:3993`/`:4002`, inside
  `#if (_WIN32_WINNT >= 0x0502)` (`:3979`, closed at `:4022`) within the same `WINVER >= 0x0500` + Desktop-partition block
  (`:3942`, `:3940`) as this session's pair; `HPOWERNOTIFY`, which both signatures name, is typedef'd at `:3985` inside that
  same 0x0502 guard. So the section is 2 declarations, 0 definitions, 0 owning TUs: the folder is missing a file, not just a
  body. Resolving T3 means either writing the two definitions (a new `powermanagement.cpp`, which also needs an
  `add_library` line, and the `static` question should be settled with them, not after) or deleting `user32.h:685-687`; both
  move the published surface, so it belongs with the U4/U13 sweep rather than with a TU review. Handle contract for whoever
  takes it: same shape as this session's pair - the returned `HPOWERNOTIFY` is released only by
  `UnregisterPowerSettingNotification`, and the GUID argument must be a registered power-setting GUID; this library exports
  neither those GUID constants nor the `POWER_BROADCAST` payload types. **Section-coverage note:** the SDK's power-notification
  family in this SDK is four entry points - the two above plus `RegisterSuspendResumeNotification` (`:4009`, ordinal 2F6) and
  `UnregisterSuspendResumeNotification` (`:4017`, ordinal 3CD) - and this header declares **2 of 4**. (`SetWindowPowerManagementNotification`
  does not exist in SDK 10.0.26100 at all: zero hits for `PowerManagement` in `winuser.h`, and it is not in the export table.)


- **T4 - the menu half of the context-help-id family is declared and defined nowhere.** Opened 31 Oct 2026 (session 16)
  from the `//Shell functions` block: `user32.h:642` `SetMenuContextHelpId(HMENU, DWORD)` and `user32.h:643`
  `GetMenuContextHelpId(HMENU)` are declared **without `static`** (the only 2 of that block's 5 members so declared) and
  **no `.cpp` in the folder defines either** - the only hits for both names outside the header are session 16's own TU
  comment in `help.cpp`. Confirmed at the binary level, not just by source search: `dumpbin /linkermember:1` on the built
  `cmakebuild/.../Debug/winqor_api_user.lib` lists exactly three help-family members
  (`?SetWindowContextHelpId@User32@api@win@qor@@SAHPEAUHWND__@@K@Z`,
  `?GetWindowContextHelpId@User32@api@win@qor@@SAKPEAUHWND__@@@Z`,
  `?WinHelp@User32@api@win@qor@@SAHPEAUHWND__@@PEB_WI_K@Z`) and **no menu pair**
  (`tmp/_tmp_help_linkermember.txt`). Both entry points are real single exports - `SetMenuContextHelpId` 34A,
  `GetMenuContextHelpId` 17C, both **RESOLVED** by the live probe - and the SDK declares both at
  `winuser.h:9135-9140` / `:9142-9146`, in the same `WINVER >= 0x0400` + Desktop block as the window pair that **is**
  wrapped, so nothing external argues against the wrappers. A caller today is ill-formed (`C2352`: a non-static member
  cannot be qualified through the class name) rather than facing `LNK2019`, which is the only way this pair differs from
  T1/T3. Fix is symmetric: add the two definitions to `help.cpp` (with `static`, matching `:644-645`) or delete
  `user32.h:642-643`. Neither touches an ABI, both move the published surface, so it belongs with the U4/U13 sweep.
  Contract for whoever adds them: the id is a plain `DWORD` property of the menu (0 clears it), no handle is acquired, and
  `GetMenuContextHelpId`'s `DWORD` return makes failure indistinguishable from a legitimate 0 - the same reason the
  window pair gets no `CheckReturn`.

Remaining queue (21 TUs, alphabetical; `accelerator.cpp`, `accessibility.cpp`, `brush.cpp`, `caret.cpp`,
`clipboard.cpp`, `combobox.cpp`, `console.cpp`, `timer.cpp`, `coordtransform.cpp`, `cursor.cpp`, `desktop.cpp`,
`devicecontext.cpp`, `devicemanagement.cpp`, `dialog.cpp`, `errorhandling.cpp`, `filledshapes.cpp`, `help.cpp` done):
hook, icon, ime, keyboard, mdi, menu, message, monitor, mouse, painting,
processthread, rectangle, resource, scrollbar, systemshutdown, text, window, windowclass, windowprocedure,
windowproperties, winqor_api_user (module glue). `hook.cpp` is in the build (see U3) and is next.

