// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_FONT
#define QOR_PP_H_WINDOWS_GUI_FONT

#include "src/platform/compiler/compiler.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/newref.h"
#include "src/platform/os/windows/common/structures.h"
#include "src/platform/os/windows/common/handles/handle.h"
#include "gdiobject.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    constexpr int LFFaceSize = 32;
    constexpr int LFFullFaceSize = 64;
    constexpr int MMMaxNumAxes = 16;

    struct LogFont
    {
        long      lfHeight;
        long      lfWidth;
        long      lfEscapement;
        long      lfOrientation;
        long      lfWeight;
        byte      lfItalic;
        byte      lfUnderline;
        byte      lfStrikeOut;
        byte      lfCharSet;
        byte      lfOutPrecision;
        byte      lfClipPrecision;
        byte      lfQuality;
        byte      lfPitchAndFamily;
        TCHAR     lfFaceName[LFFaceSize];
    };

    struct EnumLogFontEx
    {
        LogFont     elfLogFont;
        TCHAR       elfFullName[LFFullFaceSize];
        TCHAR       elfStyle[LFFaceSize];
        TCHAR       elfScript[LFFaceSize];
    };

    struct DesignVector
    {
        unsigned long  dvReserved;
        unsigned long  dvNumAxes;
        long   dvValues[MMMaxNumAxes];
    };

    struct EnumLogFontExDV
    {
        EnumLogFontEx elfEnumLogfontEx;
        DesignVector   elfDesignVector;
    };

    class qor_pp_module_interface(QOR_WINGUI) Font : public GDIObject
    {
    public:
        Font();
        Font(const PrimitiveHandle& h, bool takeOwnership);
        virtual ~Font();

        LogFont GetLogFont() const;
        static ref_of<Font>::type Create(int height, int width, int escapement, int orientation, int weight, unsigned long italic, unsigned long underline, unsigned long strikeOut, unsigned long charSet, unsigned long outputPrecision, unsigned long clipPrecision, unsigned long quality, unsigned long pitchAndFamily, const tstring& lpszFace);
        static ref_of<Font>::type Create(const LogFont& font);
        static ref_of<Font>::type Create(const EnumLogFontExDV* enumlfex);
    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_FONT