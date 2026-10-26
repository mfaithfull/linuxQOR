winqor_gui provides a Windows native GUI class library

This library implements the portable GUI concepts of the QOR, specifically for the Windows Operating System

It should provide factories, controllers and window classes for all built in Window types and Windows Common Controls. Also for OpenGL/EGL windows

It should provide fullt customizable message handling that covers all un/documented Window messages. It should support, hooks, Owner Draw, Custom Draw, Themes and Off Screen Rendering with Composition to an Open GL target window so that we can combine standrd windows controls with GL elements, transparency, non rectilenear windows and animation.

Windows UI message loop support
Windows UI message loops are inherently tied to a single thread so while we may have multiple UI threads, even Window is bound to exactly one thread. All its messages must be handled on that thread and all its rendering takes place on that thread using GDI objects bound to that thread.
We use a PerThread object to ecapsulate this per thread state. The PerThread owns a MessageHandler which implements the message loop idiom.

Message handling is filtered through a customizable tree of objects derived from BaseWindowPartHandler. Each BaseWindowPartHandler either handles the message itself, handles it by passing it to its children, or gives up and its parent tries the next sibling.
