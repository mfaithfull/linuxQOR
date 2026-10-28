winqor_gui provides a Windows native GUI class library

This library implements the portable GUI concepts of the QOR, specifically for the Windows Operating System

It should provide factories, controllers and window classes for all built in Window types and Windows Common Controls. Including for OpenGL/EGL windows

It should provide fully customizable message handling that covers all documented & undocumented Window messages. It should support, hooks, Owner Draw, Custom Draw, Themes and Off Screen Rendering with Composition to an Open GL target window so that we can combine standard windows controls with GL elements, transparency, non rectangular windows and animation.
It should surface Windows Accessibility and IME features
Have the capability to follow the users preference for dark/light theme

Windows UI message loop support
Windows UI message loops are inherently tied to a single thread so while we may have multiple UI threads, even Window is bound to exactly one thread. All its messages must be handled on that thread and all its rendering takes place on that thread using GDI objects bound to that thread.
We use a PerThread object to ecapsulate this per thread state. The PerThread owns a MessageHandler which implements the message loop idiom.

Message handling is filtered through a customizable tree of objects derived from BaseWindowPartHandler. Each BaseWindowPartHandler either handles the message itself, handles it by passing it to its children, or gives up and its parent tries the next sibling.
