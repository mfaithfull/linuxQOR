# T1-T8 deferred tasks - all eight CLOSED

- **T1 - two declared-but-undefined members in the "Authorization functions" section.** CLOSED 8 Nov 2026 (D22).

- **T2 - the whole Window Station / Desktop surface is dead exported API, and its conventions are therefore untestable
  against a caller.** CLOSED 8 Nov 2026 (D25).

- **T3 - the "Power Management functions" pair is declared but has no owning TU at all.** CLOSED 8 Nov 2026.

- **T4 - the menu half of the context-help-id family is declared and defined nowhere.** CLOSED 8 Nov 2026.

Note - the 38-TU review queue that these tasks were tracked against closed in full at session 37 (5 Nov 2026), so its queue arithmetic and its per-TU hand-offs are dropped: the folder is now 46 sources with no next review TU, and every hand-off lives in the register that names it.

- **T5 - the IME/WINNLS block in the unlabelled `user32.h` run is declared and defined nowhere.** CLOSED 8 Nov 2026 (D23).

- **T6 - the whole "//List Box functions" section is declared and defined nowhere - there is no owning TU.** CLOSED
  8 Nov 2026 (D24).

- **T7 - the dead `#include "../returncheck.h"` includes across the folder.** CLOSED 5 Nov 2026 (D16).

- **T8 - execute U18: make every wrapper's published signature match the export it binds, and close the named coverage gap.** CLOSED 7 Nov 2026 (D19 + D20).
