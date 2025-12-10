
#include </usr/include/X11/Intrinsic.h>
#include </usr/include/X11/StringDefs.h>
#include </usr/include/X11/Xaw/Label.h>

int main( int argc, char* argv[] )
{
    XtAppContext app_context;
    Widget toplevel, hello;

    toplevel = XtVaAppInitialize(
        &app_context,
        "Hello",
        NULL,
        0,
        &argc,
        argv,
        NULL,
        NULL);

    hello = XtVaCreateManagedWidget( "hello", labelWidgetClass, toplevel, NULL);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
    return 0;
}