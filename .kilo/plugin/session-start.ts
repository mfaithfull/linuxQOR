import { appendFile } from "node:fs/promises"
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
