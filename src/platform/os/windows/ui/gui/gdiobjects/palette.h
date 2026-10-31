// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_PALETTE
#define QOR_PP_H_WINDOWS_GUI_PALETTE

#include "src/platform/compiler/compiler.h"
#include "src/platform/os/windows/common/handles/handle.h"
#include "src/platform/os/windows/common/structures.h"
#include "gdiobject.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    struct PaletteEntry
    {
        byte    peRed;
        byte    peGreen;
        byte    peBlue;
        byte    peFlags;
    };

    struct LogPalette
    {
        unsigned short  palVersion;
        unsigned short  palNumEntries;
        PaletteEntry    palPalEntry[1];
    };

    class qor_pp_module_interface(QOR_WINGUI) Palette : public GDIObject
    {
    public:

        Palette();
        Palette(const PrimitiveHandle& h, bool takeOwnerShip);
        virtual ~Palette();
        const Handle& GetHandle() const;
        bool Animate(unsigned int iStartIndex, unsigned int cEntries, const PaletteEntry* ppe);
        static Palette CreateHalftone(const Handle& deviceContext);
        static Palette Create(const LogPalette* lplgpl);
        static Palette Default();
        unsigned int GetNearestIndex(unsigned long crColor);
        unsigned int GetEntries(unsigned int iStartIndex, unsigned int nEntries, PaletteEntry* lppe);
        bool Resize(unsigned int nEntries);
        //Handle Select(Handle& hdc, bool bForceBackground);
        unsigned int SetEntries(unsigned int iStart, unsigned int cEntries, const PaletteEntry* lppe);

    protected:

        Handle m_handle;
    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_PALETTE