# Executor / shell rules for this workspace (verified 26 Oct 2026)

These are observed defects of the `run_commands` executor in this workspace, not PowerShell language rules.
Each was reproduced on this machine (pwsh 7.6.9, Windows 11, MSVC/CMake 4.2.0). Follow them before writing
any command.

## Rule 1 - never use `$_` in a command string

The executor rewrites `$_` into `$'` before the text reaches pwsh, so every script block that uses the
automatic variable dies with a syntax error at run time.

    BAD : Get-ChildItem *.h | Where-Object { $_.Name -match 'thread' }
    OK  : Get-ChildItem *.h | Where-Object { $PSItem.Name -match 'thread' }
    OK  : Get-ChildItem -Filter '*.h'                  (no script block at all)

`$PSItem` passes through untouched and is the drop-in replacement for `$_`. Also prefer filter or
`Select-String` shorthand over script blocks where possible - fewer chances to mangle.

## Rule 2 - array elements of `run_commands` run concurrently, not sequentially

`commands: [ A, B ]` starts A and B at the same time. A "write then check" pair therefore usually reports
a false "file not found" / "command not found" failure, which looks like a real error but is a race.

    BAD : commands: [ "python make_patch.py", "Select-String -Path patched.cpp -Pattern x" ]
    OK  : commands: [ "python make_patch.py ; Select-String -Path patched.cpp -Pattern x" ]

Put only logically independent commands in the array. Anything with an ordering or data dependency goes
inside ONE string, sequenced with `;`.

## Rule 3 - long-running commands report "completion could not be observed"

`cmake --build` and MSBuild runs usually return with "Command completion could not be observed" and the
captured terminal text may belong to an earlier command. Never read that as success, and never read a
non-zero exit in that blob as failure.

    OK  : cmake --build cmakebuild --target X --parallel 4 > tmp/build.log 2>&1 ; 'exit=' + $LASTEXITCODE

Then in a SEPARATE call verify out of band: the log file exists, grep it for `error C` / `fatal error`,
and check the artifact timestamp moved forward.

## Rule 4 - the editor's `insert_line` inserts BEFORE that line, and `\t` in new_text is literal

`insert_line: N` puts the new text in front of the current line N, so repeating line N at the end of
`new_text` duplicates it. The observed symptom was a doubled function signature in `kthreadpool.cpp`
that compiled as `error C2761: redeclaration of member is not allowed`.

Also: a backslash-t typed inside a tool-call string reaches the file as the two characters `\` and `t`,
not a tab, which breaks C++ indentation-sensitive reads. Emit real tab characters, or re-read and fix the
prefix in a follow-up edit.

## Rule 5 - multi-line `old_text` can fail to match on CRLF files

Short single-line `old_text` matched; the same text spanning two or three lines reported "text not found"
in `kprocess.cpp` (a CRLF file) while the identical pattern worked in `kthreadpool.cpp`. When a multi-line
match fails, split it into single-line edits rather than fighting the whitespace.

## Rule 6 - audit / analysis tools that take file paths as argv

`tmp/_tmp_audit4.cpp` takes `audit <kernel.h> <k32dump> <ntdump> [implFile ...]`. Absolute paths produced
by `Get-ChildItem` (backslashes) load as EMPTY - the run prints `impl tokens: 0` and then flags EVERY
declared name MISSING, which looks like a real regression. Pass relative paths with forward slashes
(`src/platform/.../kernel/kthreadpool.cpp`); then the token count is non-zero and the SUMMARY is trustworthy.

