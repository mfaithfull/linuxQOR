A graphical UI, not necessarily a desktop
We'll have at least a primary graphical window we can draw on.
This needs to work in mobile/android style environment where that window is borderless and fixed as well as a floating window desktop environment where that window is overlayed and underlayed.

qor::ui::gui::

EGL based GUI using the existing canvas renderer
Parts will need derived implementations on GDI, X, Wayland
In particular the TopLevelContainer. The rest shouldbe generic

Display
Renderer
Toolkit
SurfaceBuffer
TopLevelContainer