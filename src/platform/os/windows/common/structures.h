// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_COMMON_STRUCTURES
#define QOR_PP_H_OS_WINDOWS_COMMON_STRUCTURES

#include "src/platform/compiler/compiler.h"
#include "src/platform/os/windows/ui/gui/view/drawing/rect.h"

struct ImageDOSHeader
{												// DOS .EXE header
    unsigned short e_magic;                     // Magic number
    unsigned short e_cblp;                      // Bytes on last page of file
    unsigned short e_cp;                        // Pages in file
    unsigned short e_crlc;                      // Relocations
    unsigned short e_cparhdr;                   // Size of header in paragraphs
    unsigned short e_minalloc;                  // Minimum extra paragraphs needed
    unsigned short e_maxalloc;                  // Maximum extra paragraphs needed
    unsigned short e_ss;                        // Initial (relative) SS value
    unsigned short e_sp;                        // Initial SP value
    unsigned short e_csum;                      // Checksum
    unsigned short e_ip;                        // Initial IP value
    unsigned short e_cs;                        // Initial (relative) CS value
    unsigned short e_lfarlc;                    // File address of relocation table
    unsigned short e_ovno;                      // Overlay number
    unsigned short e_res[4];                    // Reserved words
    unsigned short e_oemid;                     // OEM identifier (for e_oeminfo)
    unsigned short e_oeminfo;                   // OEM information; e_oemid specific
    unsigned short e_res2[10];                  // Reserved words
    long e_lfanew;								// File address of new exe header
};

#ifdef __MINGW32__
#	define __ImageBase _image_base__
#endif

namespace qor{ namespace platform { namespace win {

    typedef union _LARGE_INTEGER {
        struct {
            unsigned long LowPart;
            long HighPart;
        } DUMMYSTRUCTNAME;
        struct {
            unsigned long LowPart;
            long HighPart;
        } u;
        __int64 QuadPart;
    } LARGE_INTEGER;

    typedef union _FileSegmentElement 
    {
        void*__ptr64 Buffer;
        unsigned long long Alignment;
    }FileSegmentElement;

#if qor_pp_unicode
    typedef wchar_t TCHAR;   
    typedef std::wstring tstring;
#   define qor_pp_text(quote) L##quote
#else
    typedef char TCHAR;
    typedef std::string tstring;
#   define qor_pp_text(quote) quote
#endif 

    typedef enum _FileInfoByHandleClass
    {
        FileBasicInfo,
        FileStandardInfo,
        FileNameInfo,
        FileRenameInfo,
        FileDispositionInfo,
        FileAllocationInfo,
        FileEndOfFileInfo,
        FileStreamInfo,
        FileCompressionInfo,
        FileAttributeTagInfo,
        FileIdBothDirectoryInfo,
        FileIdBothDirectoryRestartInfo,
        FileIoPriorityHintInfo,
        FileRemoteProtocolInfo,
        FileFullDirectoryInfo,
        FileFullDirectoryRestartInfo,
        FileStorageInfo,
        FileAlignmentInfo,
        FileIdInfo,
        FileIdExtdDirectoryInfo,
        FileIdExtdDirectoryRestartInfo,
        FileDispositionInfoEx,
        FileRenameInfoEx,
        FileCaseSensitiveInfo,
        FileNormalizedNameInfo,
        MaximumFileInfoByHandleClass
    } FileInfoByHandleClass;

    #define FileInvalidFileID               ((long long)-1LL) 
    typedef struct _FileID128
    {
        unsigned char Identifier[16];                                   
    } FileID128;

    typedef struct _GUID 
    {
        unsigned long  Data1;
        unsigned short Data2;
        unsigned short Data3;
        unsigned char  Data4[ 8 ];
    } GUID;

    enum FileIdType {
        FileType,
        ObjectType,
        ExtendedFileType,
        MaximumFileType
    };

    typedef struct FileIDDescriptor 
    {
        unsigned long size;  // Size of the struct
        FileIdType Type; // Describes the type of identifier passed in.
        union {
            LARGE_INTEGER FileId;
            GUID ObjectId;
            FileID128 ExtendedFileId;
        } DUMMYUNIONNAME;
    } FileIDDescriptor;

    constexpr int OFS_MaxPathName = 128;
    typedef struct _OFStruct
    {
        unsigned char cBytes;
        unsigned char fFixedDisk;
        unsigned short nErrCode;
        unsigned short Reserved1;
        unsigned short Reserved2;
        char szPathName[OFS_MaxPathName];
    } OFStruct;

    typedef struct _FileTime 
    {
        unsigned long lowDateTime;
        unsigned long highDateTime;
    } FileTime;

    typedef struct _ByHandleFileInformation
    {
        unsigned long fileAttributes;
        FileTime ftCreationTime;
        FileTime ftLastAccessTime;
        FileTime ftLastWriteTime;
        unsigned long volumeSerialNumber;
        unsigned long fileSizeHigh;
        unsigned long fileSizeLow;
        unsigned long numberOfLinks;
        unsigned long fileIndexHigh;
        unsigned long fileIndexLow;
    } ByHandleFileInformation;

    struct DisplayDevice
    {
        unsigned long  cb{sizeof(DisplayDevice)};
        TCHAR  DeviceName[32];
        TCHAR  DeviceString[128];
        unsigned long  StateFlags;
        TCHAR  DeviceID[128];
        TCHAR  DeviceKey[128];
    };

    constexpr int CchFormName = 32;
    constexpr int CchDeviceName = 32;

    struct PointL
    {
        long  x;
        long  y;
    };

    struct DeviceMode 
    {
        TCHAR dmDeviceName[CchDeviceName];
        unsigned short dmSpecVersion;
        unsigned short dmDriverVersion;
        unsigned short dmSize;
        unsigned short dmDriverExtra;
        unsigned long dmFields;
        union 
        {
            //printer only fields
            struct 
            {
                short dmOrientation;
                short dmPaperSize;
                short dmPaperLength;
                short dmPaperWidth;
                short dmScale;
                short dmCopies;
                short dmDefaultSource;
                short dmPrintQuality;
            }_printer_fields;
            // display only fields
            struct 
            {
                PointL dmPosition;
                unsigned long  dmDisplayOrientation;
                unsigned long  dmDisplayFixedOutput;
            }_display_fields;
        };
        short dmColor;
        short dmDuplex;
        short dmYResolution;
        short dmTTOption;
        short dmCollate;
        TCHAR  dmFormName[CchFormName];
        unsigned short   dmLogPixels;
        unsigned long  dmBitsPerPel;
        unsigned long  dmPelsWidth;
        unsigned long  dmPelsHeight;
        union 
        {
            unsigned long  dmDisplayFlags;
            unsigned long  dmNup;
        };
        unsigned long  dmDisplayFrequency;    
        unsigned long  dmICMMethod;
        unsigned long  dmICMIntent;
        unsigned long  dmMediaType;
        unsigned long  dmDitherType;
        unsigned long  dmReserved1;
        unsigned long  dmReserved2;
        unsigned long  dmPanningWidth;
        unsigned long  dmPanningHeight;
    };
    
    struct XForm
    {
        float   eM11;
        float   eM12;
        float   eM21;
        float   eM22;
        float   eDx;
        float   eDy;
    };

    struct DrawTextParams
    {
        unsigned int    cbSize;
        int             iTabLength;
        int             iLeftMargin;
        int             iRightMargin;
        unsigned int    uiLengthDrawn;
    };

    struct NotificationMessageHeader
    {
        void*				m_phWindowFrom;
        unsigned long long  m_uiIdFrom;
        unsigned int		m_uiCode;
    };

    struct DeleteItemStruct
    {
        unsigned int        CtlType;
        unsigned int        CtlID;
        unsigned int        itemID;
        void*			    phwndItem;
        unsigned long long  itemData;
    };

    struct ComparisonItem
    {
        unsigned int        CtlType;
        unsigned int        CtlID;
        void*				phwndItem;
        unsigned int        itemID1;
        unsigned long long	itemData1;
        unsigned int        itemID2;
        unsigned long long  itemData2;
        unsigned long       dwLocaleId;
    };

    struct MeasureItemStruct
    {
        unsigned int       CtlType;
        unsigned int       CtlID;
        unsigned int       itemID;
        unsigned int       itemWidth;
        unsigned int       itemHeight;
        unsigned long long itemData;
    };

    struct InitCommonControlsEx
    {
        unsigned long dwSize;             // size of this structure
        unsigned long dwICC;              // flags indicating which classes to be initialized
    };

    typedef unsigned long long (__stdcall * FindReplaceHookProc) (void*, unsigned int, unsigned long long , long long);

    struct FindReplace
    {
        unsigned long       lStructSize;        // size of this struct 0x20
        void*               hwndOwner;          // handle to owner's window
        void*               hInstance;          // instance handle of.EXE that
        //   contains cust. dlg. template
        unsigned long       Flags;              // one or more of the FR_??
        TCHAR*              lpstrFindWhat;      // ptr. to search string
        TCHAR*              lpstrReplaceWith;   // ptr. to replace string
        unsigned short      wFindWhatLen;       // size of find buffer
        unsigned short      wReplaceWithLen;    // size of replace buffer
        long long           lCustData;          // data passed to hook fn.
        FindReplaceHookProc lpfnHook;           // ptr. to hook fn. or NULL
        const TCHAR*        lpTemplateName;     // custom template name
    };

    struct PointFloat 
    {
        float   x;
        float   y;
    };

    struct GlyphMetricsFloat
    {
        float       gmfBlackBoxX;
        float       gmfBlackBoxY;
        PointFloat  gmfptGlyphOrigin;
        float       gmfCellIncX;
        float       gmfCellIncY;
    };

    constexpr int wglFontLines = 0;
    constexpr int wglFontPolygons = 1;

    struct LayerPlaneDescriptor
    { 
        unsigned short  nSize;
        unsigned short  nVersion;
        unsigned long dwFlags;
        unsigned char  iPixelType;
        unsigned char  cColorBits;
        unsigned char  cRedBits;
        unsigned char  cRedShift;
        unsigned char  cGreenBits;
        unsigned char  cGreenShift;
        unsigned char  cBlueBits;
        unsigned char  cBlueShift;
        unsigned char  cAlphaBits;
        unsigned char  cAlphaShift;
        unsigned char  cAccumBits;
        unsigned char  cAccumRedBits;
        unsigned char  cAccumGreenBits;
        unsigned char  cAccumBlueBits;
        unsigned char  cAccumAlphaBits;
        unsigned char  cDepthBits;
        unsigned char  cStencilBits;
        unsigned char  cAuxBuffers;
        unsigned char  iLayerPlane;
        unsigned char  bReserved;
        unsigned long crTransparent;
    };

    struct WGLSwap
    {
        void* hdc;
        unsigned int uiFlags;
    };

    //Pixel format descriptor
    struct PixelFormatDescriptor
    {
        unsigned short  nSize{sizeof(PixelFormatDescriptor)};
        unsigned short  nVersion{1};
        unsigned long dwFlags{0};
        unsigned char  iPixelType{0};
        unsigned char  cColorBits{0};
        unsigned char  cRedBits{0};
        unsigned char  cRedShift{0};
        unsigned char  cGreenBits{0};
        unsigned char  cGreenShift{0};
        unsigned char  cBlueBits{0};
        unsigned char  cBlueShift{0};
        unsigned char  cAlphaBits{0};
        unsigned char  cAlphaShift{0};
        unsigned char  cAccumBits{0};
        unsigned char  cAccumRedBits{0};
        unsigned char  cAccumGreenBits{0};
        unsigned char  cAccumBlueBits{0};
        unsigned char  cAccumAlphaBits{0};
        unsigned char  cDepthBits{0};
        unsigned char  cStencilBits{0};
        unsigned char  cAuxBuffers{0};
        unsigned char  iLayerType{0};
        unsigned char  bReserved{0};
        unsigned long dwLayerMask{0};
        unsigned long dwVisibleMask{0};
        unsigned long dwDamageMask{0};
    };

    struct DevBroadcastDeviceInterface
    {
        unsigned long       dbcc_size;
        unsigned long       dbcc_devicetype;
        unsigned long       dbcc_reserved;
        GUID                dbcc_classguid;
        TCHAR               dbcc_name[1];
    };

    struct DevBroadcastHeader
    {
        unsigned long       dbch_size;
        unsigned long       dbch_devicetype;
        unsigned long       dbch_reserved;
    };

    typedef void (qor_pp_compiler_stdcallconvention* TimerProc)(void*, unsigned int, unsigned long long, unsigned long);

    struct PaintStruct
    {
        void*           hdc;
        int             fErase;
        Rect            rcPaint;
        int             fRestore;
        int             fIncUpdate;
        unsigned char   rgbReserved[32];
    };

    struct HelpInfo                                     // Structure pointed to by lParam of WM_HELP
    {
        unsigned int    cbSize{sizeof(HelpInfo)};       // Size in bytes of this struct
        int     iContextType;                           // Either HELPINFO_WINDOW or HELPINFO_MENUITEM
        int     iCtrlId;                                // Control Id or a Menu item Id.
        void*  hItemHandle;                             // hWnd of control or hMenu.
        unsigned long long dwContextId;                 // Context Id associated with this item
        Point   MousePos;                               // Mouse Position in screen co-ordinates
    };

    struct CompareItemStruct
    {
        unsigned int            CtlType;
        unsigned int            CtlID;
        void*                   hwndItem;
        unsigned int            itemID1;
        unsigned long long      itemData1;
        unsigned int            itemID2;
        unsigned long long      itemData2;
        unsigned long           dwLocaleId;
    };

    struct SecurityAttributes 
    {
        unsigned long length;
        void* securityDescriptor;
        int inheritHandle;
    };

    struct Coord
    {
        short x;
        short y;
    };

	struct SmallRect
	{
		short left;
		short top;
		short right;
		short bottom;
	};

	struct ConsoleSelectionInfo
	{
		unsigned long flags;
		Coord selectionAnchor;
		SmallRect srSelection;
	};

	struct ConsoleHistoryInfo
	{
		unsigned int cbSize{sizeof(ConsoleHistoryInfo)};
		unsigned int historyBufferSize;
		unsigned int numberOfHistoryBuffers;
		unsigned long flags;
	};

    struct ConsoleScreenBufferInfo
    {
        Coord size;
        Coord cursorPosition;
        unsigned short  attributes;
        SmallRect srWindow;
        Coord maximumWindowSize;
    };

    struct ConsoleScreenBufferInfoEx
    {
        unsigned long cbSize{sizeof(ConsoleScreenBufferInfoEx)};
        Coord size;
        Coord cursorPosition;
        unsigned short attributes;
        SmallRect srWindow;
        Coord maximumWindowSize;
        unsigned short popupAttributes;
        int fullscreenSupported;
        unsigned long colourTable[16];
    };

    struct CharInfo
    {
        union {
            wchar_t UnicodeChar;
            char   AsciiChar;
        } Char;
        unsigned short attributes;
    };

    struct ConsoleFontInfo
    {
        unsigned long font;
        Coord  fontSize;
    };

    struct ConsoleFontInfoEx
    {
        unsigned long cbSize{sizeof(ConsoleFontInfoEx)};
        unsigned long font;
        Coord fontSize;
        unsigned int FontFamily;
        unsigned int FontWeight;
        wchar_t FaceName[32];
    };

    struct KeyEventRecord 
    {
        int bKeyDown;
        unsigned short wRepeatCount;
        unsigned short wVirtualKeyCode;
        unsigned short wVirtualScanCode;
        union {
            wchar_t UnicodeChar;
            char   AsciiChar;
        } uChar;
        unsigned long dwControlKeyState;
    };

    struct MouseEventRecord 
    {
        Coord dwMousePosition;
        unsigned long dwButtonState;
        unsigned long dwControlKeyState;
        unsigned long dwEventFlags;
    };

    struct WindowBufferSizeRecord
    {
        Coord dwSize;
    };

    struct MenuEventRecord
    {
        unsigned int dwCommandId;
    };

    struct FocusEventRecord
    {
        int bSetFocus;
    };

    struct InputRecord 
    {
        unsigned short EventType;
        union 
        {
            KeyEventRecord KeyEvent;
            MouseEventRecord MouseEvent;
            WindowBufferSizeRecord WindowBufferSizeEvent;
            MenuEventRecord MenuEvent;
            FocusEventRecord FocusEvent;
        } Event;
    };

    struct ConsoleReadControl
    {
        unsigned long nLength;
        unsigned long nInitialChars;
        unsigned long dwCtrlWakeupMask;
        unsigned long dwControlKeyState;
    };

    struct ConsoleCursorInfo
    {
        unsigned long  dwSize;
        int   bVisible;
    };

}}}//qor::platform::win

#endif//QOR_PP_H_OS_WINDOWS_COMMON_STRUCTURES