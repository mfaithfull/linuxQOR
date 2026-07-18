User interaction that can take place with nothing more than a bidirectional pipe for the code to talk to.
what is sent out is assumed to be seen by the user.
what comes back is assumed to be keyboard input.
The output is the titles in the embedding hierarchy of the currently focussed control
e.g.
AppTitle
Main Menu
Open

followed by navigation

/:Open <:Previous ^:System Menu >:Next v:Close

Giving a list of the key strokes that are being listened for which might include 
Free text (Esc to leave).
or other instructions

followed by the state of the currently focussed control, which could be the test of an edit or just
[ selected ] 

Then we collect a keystroke and move on to the next frame to do it all again.

The concept is that this should operate with the same backend models and ultimately have the same capability as a full graphical UI. Just be much slower to use and simplified down to an absolute minimum in the presentation and input.

qor::ui::con

Display
Renderer
Toolkit
SurfaceBuffer
TopLevelContainer

