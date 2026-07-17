// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_COMMON_CONSTANTS
#define QOR_PP_H_OS_WINDOWS_COMMON_CONSTANTS


#define Invalid_Handle_Value ((void* const)(size_t)(-1))
#define Infinite_Timeout            0xFFFFFFFF  // Infinite timeout

#define Success                    0L

#define Status_Wait0        ((unsigned long)0x00000000L)
#define Wait_Failed 		((unsigned long)0xFFFFFFFF)
#define Wait_Object0       	((Status_Wait0 ) + 0 )
#define MaxPath          			260

/* Font Families */
#define FF_DontCare         0x00    /* Don't care or don't know. */
#define FW_Normal           400

// Input Mode flags:
#define EnableProcessedInput              	0x0001
#define EnableLineInput                   	0x0002
#define EnableEchoInput                   	0x0004
#define EnableWindowInput                 	0x0008
#define EnableMouseInput                  	0x0010
#define EnableInsertMode                  	0x0020
#define EnableQuickEditMode              	0x0040
#define EnableExtendedFlags		            0x0080
#define EnableAutoPosition	                0x0100
#define EnablevirtualTerminalInput			0x0200

namespace qor{ namespace platform { namespace win {

	static constexpr unsigned long Std_Input_Handle = ((unsigned long)-10);
	static constexpr unsigned long Std_Output_Handle = ((unsigned long)-11);
	static constexpr unsigned long Std_Error_Handle = ((unsigned long)-12);
/*
	static constexpr unsigned long ICC_LISTVIEW_CLASSES   = 0x00000001; // listview, header
	static constexpr unsigned long ICC_TREEVIEW_CLASSES   = 0x00000002; // treeview, tooltips
	static constexpr unsigned long ICC_BAR_CLASSES        = 0x00000004; // toolbar, statusbar, trackbar, tooltips
	static constexpr unsigned long ICC_TAB_CLASSES        = 0x00000008; // tab, tooltips
	static constexpr unsigned long ICC_UPDOWN_CLASS       = 0x00000010; // updown
	static constexpr unsigned long ICC_PROGRESS_CLASS     = 0x00000020; // progress
	static constexpr unsigned long ICC_HOTKEY_CLASS       = 0x00000040; // hotkey
	static constexpr unsigned long ICC_ANIMATE_CLASS      = 0x00000080; // animate
	static constexpr unsigned long ICC_WIN95_CLASSES      = 0x000000FF;
	static constexpr unsigned long ICC_DATE_CLASSES       = 0x00000100; // month picker, date picker, time picker, updown
	static constexpr unsigned long ICC_USEREX_CLASSES     = 0x00000200; // comboex
	static constexpr unsigned long ICC_COOL_CLASSES       = 0x00000400; // rebar (coolbar) control
	static constexpr unsigned long ICC_INTERNET_CLASSES   = 0x00000800;
	static constexpr unsigned long ICC_PAGESCROLLER_CLASS = 0x00001000;   // page scroller
	static constexpr unsigned long ICC_NATIVEFNTCTL_CLASS = 0x00002000;   // native font control
	static constexpr unsigned long ICC_STANDARD_CLASSES   = 0x00004000;
	static constexpr unsigned long ICC_LINK_CLASS         = 0x00008000;
*/
	constexpr int swHide		= 0;	//Hides the window and activates another window.
	constexpr int swNormal		= 1;	//Activates and displays a window. If the window is minimized, maximized, or arranged, the system restores it to its original size and position. An application should specify this flag when displaying the window for the first time.
	constexpr int swMinimized	= 2;	//Activates the window and displays it as a minimized window.
	constexpr int swMaximized	= 3;	//Activates the window and displays it as a maximized window.
	constexpr int swNoActivate	= 4;	//Displays a window in its most recent size and position. This value is similar to SW_SHOWNORMAL, except that the window is not activated.
	constexpr int swShow		= 5;	//Activates the window and displays it in its current size and position.
	constexpr int swMinimize	= 6;	//Minimizes the specified window and activates the next top-level window in the Z order.
	constexpr int swMinNoActive	= 7;	//Displays the window as a minimized window. This value is similar to SW_SHOWMINIMIZED, except the window is not activated.
	constexpr int swShowNA		= 8;	//Displays the window in its current size and position. This value is similar to SW_SHOW, except that the window is not activated.
	constexpr int swRestore		= 9;	//Activates and displays the window. If the window is minimized, maximized, or arranged, the system restores it to its original size and position. An application should specify this flag when restoring a minimized window.
	constexpr int swDefault		=10;	//Sets the show state based on the SW_ value specified in the STARTUPINFO structure passed to the CreateProcess function by the program that started the application.
	constexpr int swForceMin	=11;	//Minimizes a window, even if the thread that owns the window is not responding. This flag should only be used when minimizing windows from a different thread.

	static const int TitleBarChildren = 5;
	static const int DeviceNameSize = 32;

	//WGL
	// LAYERPLANEDESCRIPTOR flags
	constexpr unsigned long LPDDoubleBuffer        = 0x00000001;
	constexpr unsigned long LPDStereo              = 0x00000002;
	constexpr unsigned long LPDSuppportGDI         = 0x00000010;
	constexpr unsigned long LPDSupportOpenGL      = 0x00000020;
	constexpr unsigned long LPDShareDepth         = 0x00000040;
	constexpr unsigned long LPDShareStencil       = 0x00000080;
	constexpr unsigned long LPDShareAccum         = 0x00000100;
	constexpr unsigned long LPDSwapExchange       = 0x00000200;
	constexpr unsigned long LPDSwapCopy           = 0x00000400;
	constexpr unsigned long LPDTransparent         = 0x00001000;

	constexpr int LPDTypeRGBA        = 0;
	constexpr int LPDTypeColourIndex = 1;

	// wglSwapLayerBuffers flags
	constexpr unsigned long WGLSwapMainPlane      = 0x00000001;
	constexpr unsigned long WGLSwapOverlay1       = 0x00000002;
	constexpr unsigned long WGLSwapOverlay2       = 0x00000004;
	constexpr unsigned long WGLSwapOverlay3       = 0x00000008;
	constexpr unsigned long WGLSwapOverlay4       = 0x00000010;
	constexpr unsigned long WGLSwapOverlay5       = 0x00000020;
	constexpr unsigned long WGLSwapOverlay6       = 0x00000040;
	constexpr unsigned long WGLSwapOverlay7       = 0x00000080;
	constexpr unsigned long WGLSwapOverlay8       = 0x00000100;
	constexpr unsigned long WGLSwapOverlay9       = 0x00000200;
	constexpr unsigned long WGLSwapOverlay10      = 0x00000400;
	constexpr unsigned long WGLSwapOverlay11      = 0x00000800;
	constexpr unsigned long WGLSwapOverlay12      = 0x00001000;
	constexpr unsigned long WGLSwapOverlay13      = 0x00002000;
	constexpr unsigned long WGLSwapOverlay14      = 0x00004000;
	constexpr unsigned long WGLSwapOverlay15      = 0x00008000;
	constexpr unsigned long WGLSwapUnderlay1      = 0x00010000;
	constexpr unsigned long WGLSwapUnderlay2      = 0x00020000;
	constexpr unsigned long WGLSwapUnderlay3      = 0x00040000;
	constexpr unsigned long WGLSwapUnderlay4      = 0x00080000;
	constexpr unsigned long WGLSwapUnderlay5      = 0x00100000;
	constexpr unsigned long WGLSwapUnderlay6      = 0x00200000;
	constexpr unsigned long WGLSwapUnderlay7      = 0x00400000;
	constexpr unsigned long WGLSwapUnderlay8      = 0x00800000;
	constexpr unsigned long WGLSwapUnderlay9      = 0x01000000;
	constexpr unsigned long WGLSwapUnderlay10     = 0x02000000;
	constexpr unsigned long WGLSwapUnderlay11     = 0x04000000;
	constexpr unsigned long WGLSwapUnderlay12     = 0x08000000;
	constexpr unsigned long WGLSwapUnderlay13     = 0x10000000;
	constexpr unsigned long WGLSwapUnderlay14     = 0x20000000;
	constexpr unsigned long WGLSwapUnderlay15     = 0x40000000;

	constexpr int WGLSwapMultipleMax = 16;

	constexpr unsigned long DBTDevTypeDeviceInterface = 0x00000005;  // device interface class
	constexpr unsigned long DBTDevTypeHandle = 0x00000006;  // file system handle

	constexpr unsigned long DCXWindow           	= 0x00000001;
	constexpr unsigned long DCXCache            	= 0x00000002;
	constexpr unsigned long DCXNoResetAttrs			= 0x00000004;
	constexpr unsigned long DCXClipChildren			= 0x00000008;
	constexpr unsigned long DCXClipSiblings			= 0x00000010;
	constexpr unsigned long DCXParentClip			= 0x00000020;
	constexpr unsigned long DCXExcludeRegion		= 0x00000040;
	constexpr unsigned long DCXIntersectRegion		= 0x00000080;
	constexpr unsigned long DCXExcludeUpdate		= 0x00000100;
	constexpr unsigned long DCXIntersectUpdate		= 0x00000200;
	constexpr unsigned long DCXLockWindowUpdate		= 0x00000400;
	constexpr unsigned long DCXValidate         	= 0x00200000;

	constexpr unsigned int DCActive					 = 0x0001;		//The function uses the colors that denote an active caption.
	constexpr unsigned int DCButtons				 = 0x1000;		//If set, the function draws the buttons in the caption bar (to minimize, restore, or close an application).
	constexpr unsigned int DCGradient				 = 0x0020;		//When this flag is set, the function uses COLOR_GRADIENTACTIVECAPTION (if the DC_ACTIVE flag was set) or COLOR_GRADIENTINACTIVECAPTION for the title-bar color.
																	//If this flag is not set, the function uses COLOR_ACTIVECAPTION or COLOR_INACTIVECAPTION for both colors.
	constexpr unsigned int DCIcon					 = 0x0004;		//The function draws the icon when drawing the caption text.
	constexpr unsigned int DCInButton				 = 0x0010;		//The function draws the caption as a button.
	constexpr unsigned int DCSmallCap				 = 0x0002;		//The function draws a small caption, using the current small caption font.
	constexpr unsigned int DCText					 = 0x0008;

	constexpr unsigned int SMCXSCREEN             = 0;
	constexpr unsigned int SMCYSCREEN             = 1;
	constexpr unsigned int SMCXVSCROLL            = 2;
	constexpr unsigned int SMCYHSCROLL            = 3;
	constexpr unsigned int SMCYCAPTION            = 4;
	constexpr unsigned int SMCXBORDER             = 5;
	constexpr unsigned int SMCYBORDER             = 6;
	constexpr unsigned int SMCXDLGFRAME           = 7;
	constexpr unsigned int SMCYDLGFRAME           = 8;
	constexpr unsigned int SMCYVTHUMB             = 9;
	constexpr unsigned int SMCXHTHUMB             = 10;
	constexpr unsigned int SMCXICON               = 11;
	constexpr unsigned int SMCYICON               = 12;
	constexpr unsigned int SMCXCURSOR             = 13;
	constexpr unsigned int SMCYCURSOR             = 14;
	constexpr unsigned int SMCYMENU               = 15;
	constexpr unsigned int SMCXFULLSCREEN         = 16;
	constexpr unsigned int SMCYFULLSCREEN         = 17;
	constexpr unsigned int SMCYKANJIWINDOW        = 18;
	constexpr unsigned int SMMOUSEPRESENT         = 19;
	constexpr unsigned int SMCYVSCROLL            = 20;
	constexpr unsigned int SMCXHSCROLL            = 21;
	constexpr unsigned int SMDEBUG                = 22;
	constexpr unsigned int SMSWAPBUTTON           = 23;
	constexpr unsigned int SMRESERVED1            = 24;
	constexpr unsigned int SMRESERVED2            = 25;
	constexpr unsigned int SMRESERVED3            = 26;
	constexpr unsigned int SMRESERVED4            = 27;
	constexpr unsigned int SMCXMIN                = 28;
	constexpr unsigned int SMCYMIN                = 29;
	constexpr unsigned int SMCXSIZE               = 30;
	constexpr unsigned int SMCYSIZE               = 31;
	constexpr unsigned int SMCXFRAME              = 32;
	constexpr unsigned int SMCYFRAME              = 33;
	constexpr unsigned int SMCXMINTRACK           = 34;
	constexpr unsigned int SMCYMINTRACK           = 35;
	constexpr unsigned int SMCXDOUBLECLK          = 36;
	constexpr unsigned int SMCYDOUBLECLK          = 37;
	constexpr unsigned int SMCXICONSPACING        = 38;
	constexpr unsigned int SMCYICONSPACING        = 39;
	constexpr unsigned int SMMENUDROPALIGNMENT    = 40;
	constexpr unsigned int SMPENWINDOWS           = 41;
	constexpr unsigned int SMDBCSENABLED          = 42;
	constexpr unsigned int SMCMOUSEBUTTONS        = 43;

	constexpr unsigned int SMCXFIXEDFRAME           = SMCXDLGFRAME;
	constexpr unsigned int SMCYFIXEDFRAME           = SMCYDLGFRAME;
	constexpr unsigned int SMCXSIZEFRAME            = SMCXFRAME;
	constexpr unsigned int SMCYSIZEFRAME            = SMCYFRAME;
	constexpr unsigned int SMSECURE               = 44;
	constexpr unsigned int SMCXEDGE               = 45;
	constexpr unsigned int SMCYEDGE               = 46;
	constexpr unsigned int SMCXMINSPACING         = 47;
	constexpr unsigned int SMCYMINSPACING         = 48;
	constexpr unsigned int SMCXSMICON             = 49;
	constexpr unsigned int SMCYSMICON             = 50;
	constexpr unsigned int SMCYSMCAPTION          = 51;
	constexpr unsigned int SMCXSMSIZE             = 52;
	constexpr unsigned int SMCYSMSIZE             = 53;
	constexpr unsigned int SMCXMENUSIZE           = 54;
	constexpr unsigned int SMCYMENUSIZE           = 55;
	constexpr unsigned int SMARRANGE              = 56;
	constexpr unsigned int SMCXMINIMIZED          = 57;
	constexpr unsigned int SMCYMINIMIZED          = 58;
	constexpr unsigned int SMCXMAXTRACK           = 59;
	constexpr unsigned int SMCYMAXTRACK           = 60;
	constexpr unsigned int SMCXMAXIMIZED          = 61;
	constexpr unsigned int SMCYMAXIMIZED          = 62;
	constexpr unsigned int SMNETWORK              = 63;
	constexpr unsigned int SMCLEANBOOT            = 67;
	constexpr unsigned int SMCXDRAG               = 68;
	constexpr unsigned int SMCYDRAG               = 69;
	constexpr unsigned int SMSHOWSOUNDS           = 70;
	constexpr unsigned int SMCXMENUCHECK          = 71;   // Use instead of GetMenuCheckMarkDimensions()
	constexpr unsigned int SMCYMENUCHECK          = 72;
	constexpr unsigned int SMSLOWMACHINE          = 73;
	constexpr unsigned int SMMIDEASTENABLED       = 74;
	constexpr unsigned int SMMOUSEWHEELPRESENT    = 75;
	constexpr unsigned int SMXVIRTUALSCREEN       = 76;
	constexpr unsigned int SMYVIRTUALSCREEN       = 77;
	constexpr unsigned int SMCXVIRTUALSCREEN      = 78;
	constexpr unsigned int SMCYVIRTUALSCREEN      = 79;
	constexpr unsigned int SMCMONITORS            = 80;
	constexpr unsigned int SMSAMEDISPLAYFORMAT    = 81;
	constexpr unsigned int SMIMMENABLED           = 82;
	constexpr unsigned int SMCXFOCUSBORDER        = 83;
	constexpr unsigned int SMCYFOCUSBORDER        = 84;
	constexpr unsigned int SMTABLETPC             = 86;
	constexpr unsigned int SMMEDIACENTER          = 87;
	constexpr unsigned int SMSTARTER              = 88;
	constexpr unsigned int SMSERVERR2             = 89;
	constexpr unsigned int SMMOUSEHORIZONTALWHEELPRESENT    = 91;
	constexpr unsigned int SMCXPADDEDBORDER       = 92;
	constexpr unsigned int SMDIGITIZER            = 94;
	constexpr unsigned int SMMAXIMUMTOUCHES       = 95;
	constexpr unsigned int SMREMOTESESSION        = 0x1000;
	constexpr unsigned int SMSHUTTINGDOWN           = 0x2000;
	constexpr unsigned int SMREMOTECONTROL          = 0x2001;
	constexpr unsigned int SMCARETBLINKINGENABLED   = 0x2002;
	constexpr unsigned int SMCONVERTIBLESLATEMODE   = 0x2003;
	constexpr unsigned int SMSYSTEMDOCKED           = 0x2004;

	enum WINDOWPARTS {
		WPCAPTION = 1,
		WPSMALLCAPTION = 2,
		WPMINCAPTION = 3,
		WPSMALLMINCAPTION = 4,
		WPMAXCAPTION = 5,
		WPSMALLMAXCAPTION = 6,
		WPFRAMELEFT = 7,
		WPFRAMERIGHT = 8,
		WPFRAMEBOTTOM = 9,
		WPSMALLFRAMELEFT = 10,
		WPSMALLFRAMERIGHT = 11,
		WPSMALLFRAMEBOTTOM = 12,
		WPSYSBUTTON = 13,
		WPMDISYSBUTTON = 14,
		WPMINBUTTON = 15,
		WPMDIMINBUTTON = 16,
		WPMAXBUTTON = 17,
		WPCLOSEBUTTON = 18,
		WPSMALLCLOSEBUTTON = 19,
		WPMDICLOSEBUTTON = 20,
		WPRESTOREBUTTON = 21,
		WPMDIRESTOREBUTTON = 22,
		WPHELPBUTTON = 23,
		WPMDIHELPBUTTON = 24,
		WPHORZSCROLL = 25,
		WPHORZTHUMB = 26,
		WPVERTSCROLL = 27,
		WPVERTTHUMB = 28,
		WPDIALOG = 29,
		WPCAPTIONSIZINGTEMPLATE = 30,
		WPSMALLCAPTIONSIZINGTEMPLATE = 31,
		WPFRAMELEFTSIZINGTEMPLATE = 32,
		WPSMALLFRAMELEFTSIZINGTEMPLATE = 33,
		WPFRAMERIGHTSIZINGTEMPLATE = 34,
		WPSMALLFRAMERIGHTSIZINGTEMPLATE = 35,
		WPFRAMEBOTTOMSIZINGTEMPLATE = 36,
		WPSMALLFRAMEBOTTOMSIZINGTEMPLATE = 37,
		WPFRAME = 38,
		WPBORDER = 39,
	};

	enum CAPTIONSTATES 
	{
		CSActive = 1,
		CSInactive = 2,
		CSDisabled = 3,
	};
	
	enum FRAMESTATES 
	{
		FSActive = 1,
		FSInactive = 2,
	};

	enum FRAMELEFTSTATES 
	{
		FRLActive = 1,
		FRLInactive = 2,
	};

	constexpr long long	HTError			=-2;	//On the screen background or on a dividing line between windows (same as HTNOWHERE, except that the DefWindowProc function produces a system beep to indicate an error).
	constexpr long long	HTTransparent	= -1;	//In a window currently covered by another window in the same thread (the message will be sent to underlying windows in the same thread until one of them returns a code that is not HTTRANSPARENT).
	constexpr long long	HTNowhere		= 0;	//On the screen background or on a dividing line between windows.
	constexpr long long	HTClient		= 1;	//In a client area.
	constexpr long long	HTCaption		= 2;	//In a title bar.
	constexpr long long	HTSysMenu		= 3;	//In a window menu or in a Close button in a child window.
	constexpr long long	HTSize			= 4;	//In a size box (same as HTGROWBOX).
	constexpr long long	HTGrowBox		= 4;	//In a size box (same as HTSIZE).
	constexpr long long	HTMenu			= 5;	//In a menu.
	constexpr long long	HTHScroll		= 6;	//In a horizontal scroll bar.
	constexpr long long	HTVScroll		= 7;	//In the vertical scroll bar.
	constexpr long long	HTMinButton		= 8;	//In a Minimize button.
	constexpr long long	HTReduce		= 8;	//In a Minimize button.
	constexpr long long	HTMaxButton		= 9;	//In a Maximize button.
	constexpr long long	HTZoom			= 9;
	constexpr long long	HTLeft			= 10;	//In the left border of a resizable window (the user can click the mouse to resize the window horizontally).
	constexpr long long	HTRight			= 11;	//In the right border of a resizable window (the user can click the mouse to resize the window horizontally).	
	constexpr long long	HTTop			= 12;	//In the upper-horizontal border of a window.
	constexpr long long	HTTopLeft		= 13;	//In the upper-left corner of a window border.
	constexpr long long	HTTopRight		= 14;	//In the upper-right corner of a window border.	
	constexpr long long	HTBottom		= 15;	//In the lower-horizontal border of a resizable window (the user can click the mouse to resize the window vertically).
	constexpr long long	HTBottomLeft	= 16;	//In the lower-left corner of a border of a resizable window (the user can click the mouse to resize the window diagonally).
	constexpr long long	HTBottomRight	= 17;	//In the lower-right corner of a border of a resizable window (the user can click the mouse to resize the window diagonally).
	constexpr long long	HTBorder		= 18;	//In the border of a window that does not have a sizing border.
	constexpr long long	HTClose			= 20;	//In a Close button.	
	constexpr long long	HTHelp			= 21;	//In a Help button.	

 	//DrawText() Format Flags

	constexpr unsigned long	DTTop		= 0x00000000;
	constexpr unsigned long	DTLeft		= 0x00000000;
	constexpr unsigned long	DTCenter	= 0x00000001;
	constexpr unsigned long	DTRight		= 0x00000002;
/*
#define DT_VCENTER                  0x00000004
#define DT_BOTTOM                   0x00000008
#define DT_WORDBREAK                0x00000010
#define DT_SINGLELINE               0x00000020
#define DT_EXPANDTABS               0x00000040
#define DT_TABSTOP                  0x00000080
#define DT_NOCLIP                   0x00000100
#define DT_EXTERNALLEADING          0x00000200
#define DT_CALCRECT                 0x00000400
#define DT_NOPREFIX                 0x00000800
#define DT_INTERNAL                 0x00001000
#define DT_EDITCONTROL              0x00002000
#define DT_PATH_ELLIPSIS            0x00004000
#define DT_END_ELLIPSIS             0x00008000
#define DT_MODIFYSTRING             0x00010000
#define DT_RTLREADING               0x00020000
#define DT_WORD_ELLIPSIS            0x00040000
#define DT_NOFULLWIDTHCHARBREAK     0x00080000
#define DT_HIDEPREFIX               0x00100000
#define DT_PREFIXONLY               0x00200000
*/

	// Ternary raster operations
	constexpr unsigned long	SrcCopy             =0x00CC0020; /* dest = source                   */
	constexpr unsigned long	SrcPaint            =0x00EE0086; /* dest = source OR dest           */
	constexpr unsigned long	SrcAnd              =0x008800C6; /* dest = source AND dest          */
	constexpr unsigned long	Srcinvert           =0x00660046; /* dest = source XOR dest          */
	constexpr unsigned long	SrcErase            =0x00440328; /* dest = source AND (NOT dest )   */
	constexpr unsigned long	NotSrcCopy          =0x00330008; /* dest = (NOT source)             */
	constexpr unsigned long	NotSrcErase         =0x001100A6; /* dest = (NOT src) AND (NOT dest) */
	constexpr unsigned long	MergeCopy           =0x00C000CA; /* dest = (source AND pattern)     */
	constexpr unsigned long	MergePaint          =0x00BB0226; /* dest = (NOT source) OR dest     */
	constexpr unsigned long	PatCopy             =0x00F00021; /* dest = pattern                  */
	constexpr unsigned long	PatPaint            =0x00FB0A09; /* dest = DPSnoo                   */
	constexpr unsigned long	PatInvert           =0x005A0049; /* dest = pattern XOR dest         */
	constexpr unsigned long	DstInvert           =0x00550009; /* dest = (NOT dest)               */
	constexpr unsigned long	Blackness           =0x00000042; /* dest = BLACK                    */
	constexpr unsigned long	Whiteness           =0x00FF0062; /* dest = WHITE                    */
	constexpr unsigned long	NoMirrorBitmap      =0x80000000; /* Do not Mirror the bitmap in this call */
	constexpr unsigned long	CaptureBlt          =0x40000000; /* Include layered windows */

	//PIXELFORMATDESCRIPTOR flags
	constexpr unsigned long	PFDDOUBLEBUFFER            =0x00000001;
	constexpr unsigned long	PFDSTEREO                  =0x00000002;
	constexpr unsigned long	PFDDRAWTOWINDOW          =0x00000004;
	constexpr unsigned long	PFDDRAWTOBITMAP          =0x00000008;
	constexpr unsigned long	PFDSUPPORTGDI             =0x00000010;
	constexpr unsigned long	PFDSUPPORTOPENGL          =0x00000020;
	constexpr unsigned long	PFDGENERICFORMAT          =0x00000040;
	constexpr unsigned long	PFDNEEDPALETTE            =0x00000080;
	constexpr unsigned long	PFDNEEDSYSTEMPALETTE     =0x00000100;
	constexpr unsigned long	PFDSWAPEXCHANGE           =0x00000200;
	constexpr unsigned long	PFDSWAPCOPY               =0x00000400;
	constexpr unsigned long	PFDSWAPLAYERBUFFERS      =0x00000800;
	constexpr unsigned long	PFDGENERICACCELERATED     =0x00001000;
	constexpr unsigned long	PFDSUPPORTDIRECTDRAW      =0x00002000;
	constexpr unsigned long	PFDDIRECT3DACCELERATED    =0x00004000;
	constexpr unsigned long	PFDSUPPORTCOMPOSITION     =0x00008000;

	// PIXELFORMATDESCRIPTOR flags for use in ChoosePixelFormat only
	constexpr unsigned long	PFDDEPTHDONTCARE          =0x20000000;
	constexpr unsigned long	PFDDOUBLEBUFFERDONTCARE   =0x40000000;
	constexpr unsigned long	PFDSTEREODONTCARE         =0x80000000;

	// pixel types */
	constexpr int PFDTYPERGBA         = 0;
	constexpr int PFDTYPECOLORINDEX   = 1;

	// layer types
	constexpr int PFDMAINPLANE       = 0;
	constexpr int PFDOVERLAYPLANE    = 1;
	constexpr int PFDUNDERLAYPLANE   = (-1);

	constexpr long GENERICREAD                     = (0x80000000L);
	constexpr long GENERICWRITE                    = (0x40000000L);
	constexpr long GENERICEXECUTE                  = (0x20000000L);
	constexpr long GENERICALL                      = (0x10000000L);

	constexpr unsigned long FILESHAREREAD                 = 0x00000001;
	constexpr unsigned long FILESHAREWRITE                = 0x00000002;

}}}//qor::platform::win

#endif//QOR_PP_H_OS_WINDOWS_COMMON_CONSTANTS