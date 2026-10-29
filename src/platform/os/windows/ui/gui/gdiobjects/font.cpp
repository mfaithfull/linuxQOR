// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "font.h"
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/api_layer/user/user32.h"
#include "src/platform/os/windows/api_layer/gdi/gdi32.h"

using namespace qor::win::api;

namespace qor{ namespace platform { namespace win{

    Font::Font() : GDIObject(OFont)
    {
    }

    Font::Font(const PrimitiveHandle& h, bool takeOwnership) : GDIObject(h, OFont, takeOwnership)
    {
    }

    Font::~Font()
    {
    }

    // Retrieves the LOGFONT structure that contains font attributes.
    LogFont Font::GetLogFont() const
    {
        LogFont logFont = {};
        GDI32::GetObjectT((HGDIOBJ)(GetHandle().Use()), sizeof(logFont), &logFont);
        return logFont;
    }

    ref_of<Font>::type Font::Create(int height, int width, int escapement, int orientation, int weight, unsigned long italic, unsigned long underline, unsigned long strikeOut, unsigned long charSet, unsigned long outputPrecision, unsigned long clipPrecision, unsigned long quality, unsigned long pitchAndFamily, const tstring& face)
    {
        return new_ref<Font>(PrimitiveHandle(GDI32::CreateFontT(height, width, escapement, orientation, weight, italic, underline, strikeOut, charSet, outputPrecision, clipPrecision, quality, pitchAndFamily, face.c_str())), true);
    }

    ref_of<Font>::type Font::Create(const LogFont& font)
    {
        return new_ref<Font>(PrimitiveHandle(GDI32::CreateFontIndirectT(reinterpret_cast< const ::LOGFONT*>(&font))), true);
    }

    ref_of<Font>::type Font::Create(const EnumLogFontExDV* enumlfex)
    {
        return new_ref<Font>(PrimitiveHandle(GDI32::CreateFontIndirectExT(reinterpret_cast< const ::ENUMLOGFONTEXDV*>(enumlfex))), true);
    }

}}}//qor::platform::win
