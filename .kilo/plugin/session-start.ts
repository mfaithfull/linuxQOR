// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Server plugin: the deterministic half of the what-are-we-doing-now skill, so
// the session bootstrap does not depend on the model remembering to run the
// skill. On the first user message of each top-level session it stamps
// memory-bank/now.md with the session ID and the user's prompt, puts TODAY'S
// PROJECT README, <project>/README.md, into the Resources: list, and injects a
// synthetic part telling the agent to read now.md. Today's project is the
// Project: key of memory-bank/today.md, written by what-are-we-doing-today;
// read it back, never infer it. When that project has no README.md the
// Resources: placeholder stays exactly as the template has it and the note
// says so. Never substitute the workspace root README, a sibling README, or
// purpose.md for today's project README. Child sessions (parentID set) are
// skipped. When now.md already carries this session's ID it is left untouched,
// preserving plan check-offs from earlier turns.

import { appendFile, mkdir, readFile, stat, writeFile } from "node:fs/promises"
import { join } from "node:path"
import { randomUUID } from "node:crypto"
import type { Plugin } from "@kilocode/plugin"

const DEFAULT_TEMPLATE = [
  "---",
  "Session:",
  "Prompt:",
  "#A list of files to read after compacting",
  "Resources:",
  "  -",
  "#Write the session plan here to ensure it survives compacting",
  "#Check off items as they are completed so they don't have to be rechecked",
  "#Trust the plan",
  "---",
  "",
  "Plan:",
  "",
  " - [ ] ''",
].join("\n")

const unescapeObsidian = (text: string) => text.replace(/&#x20;/g, " ").replace(/\\(?=[-#[\]])/g, "")

const quoteYaml = (value: string) => {
  const flat = value.replace(/\r?\n/g, " ").trim()
  if (/["\\]/.test(flat)) return "'" + flat.replace(/'/g, "''") + "'"
  return '"' + flat + '"'
}

const ownedBy = (note: string, sessionID: string) => new RegExp("^Session:\\s*" + sessionID + "\\s*$", "m").test(note)

const todayProject = async (worktree: string) => {
  try {
    const text = await readFile(join(worktree, "memory-bank", "today.md"), "utf8")
    const m = text.match(/^Project:\s*(.+)$/m)
    if (!m) return null
    const project = m[1].trim().replace(/^'(.*)'$/, "$1").replace(/^"(.*)"$/, "$1").trim()
    return project === "" ? null : project
  } catch {
    return null
  }
}

const resourcesEntry = async (worktree: string) => {
  const project = await todayProject(worktree)
  if (!project) return null
  const rel = project === "." ? "README.md" : project.replace(/[\\/]+$/, "") + "/README.md"
  try {
    await stat(join(worktree, ...rel.split(/[\\/]+/)))
  } catch {
    return null
  }
  return project === "." ? "./README.md" : project.replace(/\\/g, "/").replace(/\/+$/, "") + "/README.md"
}

const fillTemplate = (template: string, sessionID: string, prompt: string, resources: string | null) => {
  const lines = template.split(/\r?\n/)
  const closing = lines.findIndex((line, i) => i > 0 && line.trim() === "---")
  const limit = closing === -1 ? lines.length : closing
  for (let i = 0; i < limit; i++) {
    if (/^Session:/.test(lines[i])) lines[i] = "Session: " + sessionID
    else if (/^Prompt:/.test(lines[i])) lines[i] = "Prompt: " + quoteYaml(prompt)
  }
  if (resources) {
    const at = lines.findIndex((line, i) => i < limit && /^Resources:\s*$/.test(line))
    if (at !== -1 && at + 1 < limit && /^\s*-\s*$/.test(lines[at + 1])) {
      lines[at + 1] = (lines[at + 1].match(/^\s*/) ?? ["  "])[0] + "- '" + resources + "'"
    }
  }
  return lines.join("\n")
}

const stampNow = async (worktree: string, sessionID: string, prompt: string) => {
  const nowPath = join(worktree, "memory-bank", "now.md")
  let existing: string | null = null
  try {
    existing = await readFile(nowPath, "utf8")
  } catch {}
  if (existing && ownedBy(existing, sessionID)) return "already stamped for this session; left untouched"
  let template = DEFAULT_TEMPLATE
  try {
    template = unescapeObsidian(await readFile(join(worktree, ".kilo", "now_template.md"), "utf8"))
  } catch {}
  const resources = await resourcesEntry(worktree)
  await mkdir(join(worktree, "memory-bank"), { recursive: true })
  await writeFile(nowPath, fillTemplate(template, sessionID, prompt, resources), "utf8")
  return resources
    ? "stamped, Resources: " + resources
    : "stamped, today's project has no README.md so the Resources: placeholder was kept"
}

const sessionStart: Plugin = async ({ client, worktree }) => {
  const log = async (message: string) => {
    await client.app.log({
      body: { service: "session-start", level: "info", message },
    })
    await appendFile(join(worktree, ".kilo", "session-starts.log"), `${new Date().toISOString()} ${message}\n`)
  }

  const logWikiPending = async () => {
    // The SessionStart nudge (docs/WikiDesign.md 9(3b)): pending = N sits at session start so
    // "the wiki is behind" is a visible fact. Counted here, not by an LLM's reading of the file:
    // an entry is one line starting '- id:' at column 0 (docs/raw/pending.md, Parsing).
    try {
      const text = await readFile(join(worktree, "docs", "raw", "pending.md"), "utf8")
      const pending = text.split(/\r?\n/).filter((line) => line.startsWith("- id: ")).length
      await log(`[wiki] pending = ${pending} of ${pending}${pending > 0 ? " - run /wiki-ingest" : ""}`)
    } catch {
      await log("[wiki] pending = unknown (docs/raw/pending.md unreadable)")
    }
  }

  const stamped = new Set<string>()

  return {
    "chat.message": async (input, output) => {
      const sessionID = input.sessionID
      if (stamped.has(sessionID)) return
      stamped.add(sessionID)
      try {
        const res = await client.session.get({ path: { id: sessionID } })
        if (res.data?.parentID) return
      } catch {}
      let note: string
      try {
        const prompt = (output.parts ?? [])
          .filter((part: any) => part.type === "text" && !part.synthetic && typeof part.text === "string")
          .map((part: any) => part.text as string)
          .join("\n")
          .trim()
        note = prompt ? await stampNow(worktree, sessionID, prompt) : "no user text in the first message; now.md not stamped"
      } catch (error) {
        note = "stamping failed: " + (error instanceof Error ? error.message : String(error))
      }
      await log(`now.md ${note} (${sessionID})`)
      await logWikiPending()
      ;(output.parts as any[]).push({
        id: "prt_" + randomUUID(),
        sessionID,
        messageID: output.message.id,
        type: "text",
        synthetic: true,
        text:
          `Working-memory note memory-bank/now.md for this session (${sessionID}): ${note}. ` +
          "Read memory-bank/now.md before acting: its Resources: list names today's project README and its Plan: is the session plan. " +
          (note.startsWith("stamped")
            ? "Do not run the what-are-we-doing-now skill; just read the note."
            : "Run the what-are-we-doing-now skill to (re)stamp it, then read it."),
      })
    },
  }
}

export default { id: "session-start", server: sessionStart }
