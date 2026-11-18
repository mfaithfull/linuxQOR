
Currently:
    Getting cross platform GUI off the ground.
    Singleton DesktopGUI object which enables toolkit selection
    PerThread/GUIThread object which is toolkit specific and own per thread state, not a flyer as it doesn't need stacking
    Widget and layout kept at the QOR level with per platform implementations
    Win32 implementation has a working handler hierarchy
    Win32 layout integration needs redoing to use layout at the QOR level
    Action system for tying GUI events back to actual application architecture is critically needed
    We want a Model View Controller conceptual framework but we must have Port and Adapters architecture and we must be able to set up trivial applications trivially without hand writing multiple adapter layers to get data to and from the GUI.
    Looking at using qor::Property set as an intermediary and serializing that to JSON for layout definitions and comms

Many of the low level /qor libraries and systems are now pretty thoroughly beaten into shape and clearly working well. It's about time to start polishing and publishing them. However a lot of others are still very sketchy and may be subject to fundamental change so we don't simply want to v1 the entire repo.

We could create an upstream Repo of publishable stuff and only feed the polished bits into that. That should be the main QOR repo that needs otherwise evicerating.

It will need CI/CD for Windows/CL and Linux/GCC at least from the ground up.

The question is do we develop it with ride along tutorials, Cherno style. Each step will need it's own documentation and a checkpoint/tag so that the code for that step can be accessed later and each step still builds on the last.
This can be done with git branches, checkpoints or seperate repos but the best way needs some research.

What do we do with the series of 'The Problem With...' articles. They still need some polish and fact checking but they should go somewhere. Not in a code repo and probably not on r/cpp either.

To bootstrap a tutorial series we'll need the obligatory environment setup phase for vscode/C++/CMake on Windows and Linux which is a diversion in itself. This should definitely be a side branch that people can skip.
Doing screen capture the right way is critical for a video series. As is deciding whether to appear in it in person. The harder but correct answer is yes.

