// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_BITMAP
#define QOR_PP_H_WINDOWS_GUI_BITMAP

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/newref.h"
#include "src/platform/os/windows/common/handles/handle.h"
#include "gdiobject.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    struct RGBQUAD
    {
        unsigned char    rgbBlue{0};
        unsigned char    rgbGreen{0};
        unsigned char    rgbRed{0};
        unsigned char    rgbReserved{0};
    };

    struct BitmapInfoHeader
    {
        unsigned long       biSize{sizeof(BitmapInfoHeader)};
        long                biWidth{0};
        long                biHeight{0};
        unsigned short      biPlanes{0};
        unsigned short      biBitCount{0};
        unsigned long       biCompression{0};
        unsigned long       biSizeImage{0};
        long                biXPelsPerMeter{0};
        long                biYPelsPerMeter{0};
        unsigned long       biClrUsed{0};
        unsigned long       biClrImportant{0};
    };

    struct BitmapInfo
    {
        BitmapInfoHeader    bmiHeader;
        RGBQUAD             bmiColors[1];
    };

    // constants for the biCompression field
    constexpr unsigned long BIRGB        = 0;
    constexpr unsigned long BIRLE8       = 1;
    constexpr unsigned long BIRLE4       = 2;
    constexpr unsigned long BIBITFIELDS  = 3;
    constexpr unsigned long BIJPEG       = 4;
    constexpr unsigned long BIPNG        = 5;

    // DIB color table identifiers

    constexpr unsigned int DIBRGBCOLORS  = 0; // color table in RGBs
    constexpr unsigned int DIBPALCOLORS  = 1; // color table in palette indices

    class qor_pp_module_interface(QOR_WINGUI) DeviceContext;

    class qor_pp_module_interface(QOR_WINGUI) Bitmap : public GDIObject
    {
    public:
        Bitmap();
        Bitmap(const PrimitiveHandle& h, bool takeOwnership);
        virtual ~Bitmap();

    };

    class qor_pp_module_interface(QOR_WINGUI) DIB : public Bitmap
    {
    public:
        DIB();
        DIB(const PrimitiveHandle& h, bool takeOwnership);
        virtual ~DIB();
    };

    class qor_pp_module_interface(QOR_WINGUI) DIBSection : public Bitmap
    {
    public:
        DIBSection(ref_of<DeviceContext>::type dc, const BitmapInfo* bmpi, unsigned int usage, void** bits, void* section, unsigned long offset);
        DIBSection(const PrimitiveHandle& h, bool takeOwnership);
        virtual ~DIBSection();
    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_BITMAP