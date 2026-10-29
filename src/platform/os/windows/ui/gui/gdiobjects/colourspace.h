// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_COLOURSPACE
#define QOR_PP_H_WINDOWS_GUI_COLOURSPACE

#include "src/platform/os/windows/common/constants.h"
#include "src/platform/os/windows/common/handles/handle.h"
#include "gdiobject.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    struct CieXyz
    {
        long ciexyzX;
        long ciexyzY;
        long ciexyzZ;
    };

    struct CieXyzTriple
    {
        CieXyz  ciexyzRed;
        CieXyz  ciexyzGreen;
        CieXyz  ciexyzBlue;
    };

    struct LogColourSpace
    {
        unsigned long lcsSignature;
        unsigned long lcsVersion;
        unsigned long lcsSize;
        long lcsCSType;
        long lcsIntent;
        CieXyzTriple lcsEndpoints;
        unsigned long lcsGammaRed;
        unsigned long lcsGammaGreen;
        unsigned long lcsGammaBlue;
    #if qor_pp_unicde
        wchar_t lcsFilename[MaxPath];
    #else
        char lcsFilename[MaxPath];
    #endif
    };

    class qor_pp_module_interface(QOR_WINGUI) ColourSpace : public GDIObject
    {
    public:

        ColourSpace();
        ColourSpace(const PrimitiveHandle& h, bool takeOwnership);
        ~ColourSpace();

        static ColourSpace Create(LogColourSpace* lpLogColorSpace);
        bool Delete();
        static ColourSpace Get(const Handle& deviceContext);
        bool GetLogicalColorSpace(LogColourSpace* lpBuffer, unsigned long nSize);
        Handle Set(const Handle& deviceContext);

    protected:

        Handle m_handle;
    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_COLOURSPACE