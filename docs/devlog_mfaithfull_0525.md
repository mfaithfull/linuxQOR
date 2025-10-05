Have mostly been experimenting with class structures and approaches for doing UI in the simplest possible common context which is the Console.
This is clearly a toy system and is a set of experiments which can later be applied to other UI contexts like the Terminal or even Metal or Vulkan.

Next steps:
Take the interactive parts out of the Unit tests and give them their own executables so the tests run uninterrrupted.
Tie up running an Application with a pluggable MainLoop with Windows and Linux implementations
Get genuine async file reading and writing going on Windows and Linux through the same interface so we can write basic file processing apps
Probably write one or more processors to pull aparts the Unicode database as we have before but also to write optimised maps for lookups we might need to files
A from file resource loader for strings and images at least would be useful. Palettes too for styling. These should be simple enough.
We could do with a general character set concept with tokenization similar to what's prototyped in the ConsoleUI.
How do we best hook up a UI to a Workflow in such a way that the Workflow generates the UI and the UI drives the Workflow?

I've looked at ImGUI for both inspiration and potential inclusion. It's great but also a large mess and not easy to pick apart. Further work will be necessary before we can get value from it.
Vulkan, OpenGL, Qurses/NCurses/PDCurses and Native Win32 renderers are a goal.

