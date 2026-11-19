---
Session: ses_e0ad17999ffeesuPOscMxxVDLP
Prompt: "What approaches can you suggest to improve the way we handle EOF/EOD/EOS in pipeline elements? At the moment we can raise a signal for an OnEndOfData event from a Source but there's no clearly correct place to handle that. The overall Pipeline doesn't own that state and other, downstream, pipeline elements may still have data to process so the signal isn't that useful to them."
#A list of files to read after compacting
Resources:
  -
#Write the session plan here to ensure it survives compacting
#Check off items as they are completed so they don't have to be rechecked
#Trust the plan
---

Plan:

  - [ ] ''
