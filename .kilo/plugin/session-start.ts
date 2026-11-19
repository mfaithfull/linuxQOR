import { appendFile, readFile } from "node:fs/promises"
import { join } from "node:path"
import type { Plugin } from "@kilocode/plugin"

const seen = new Set<string>()

const sessionStart: Plugin = async ({ client, worktree }) => {
  const log = async (message: string) => {
    await client.app.log({
      body: { service: "session-start", level: "info", message },
    })
    await appendFile(join(worktree, ".kilo", "session-starts.log"), `${new Date().toISOString()} ${message}\n`)
  }

  const onStart = async (sessionID: string, origin: string) => {
    await log(`${origin} ${sessionID}`)
    await logWikiPending()
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

  return {
    event: async ({ event }) => {
      if (event.type === "session.created") {
        const info = event.properties.info
        if (info.parentID) return
        seen.add(info.id)
        await onStart(info.id, "created:")
      }
    },
    "chat.message": async (input) => {
      if (seen.has(input.sessionID)) return
      seen.add(input.sessionID)
      await onStart(input.sessionID, "resumed-first-message:")
    },
  }
}

export default { id: "session-start", server: sessionStart }
