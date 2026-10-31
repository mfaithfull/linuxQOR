// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "icon.h"
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/api_layer/user/user32.h"

using namespace qor::win::api;

namespace qor{ namespace platform { namespace win{

    Icon::Icon(const PrimitiveHandle& h, bool own) : m_handle(h.Use()), m_owning(own)
    {
    }

    Icon::Icon(const Icon& src) : m_handle( User32::CopyIcon(reinterpret_cast<HICON>(src.m_handle.Use()))), m_owning(true)
    {
    }

    Icon::Icon(void* hInstance, int nWidth, int nHeight, byte cPlanes, byte cBitsPixel, const byte* lpbANDbits, const byte* lpbXORbits)
    {
        m_handle = User32::CreateIcon(reinterpret_cast<HINSTANCE>(hInstance), nWidth, nHeight, cPlanes, cBitsPixel, lpbANDbits, lpbXORbits);
        m_owning = true;
    }

    Icon::Icon(byte* presbits, unsigned long dwResSize, bool fIcon, unsigned long dwVer)
    {
        m_handle = User32::CreateIconFromResource(presbits, dwResSize, fIcon ? 1:0, dwVer);
        m_owning = true;
    }

    Icon::Icon(byte* pbIconBits, unsigned long cbIconBits, bool fIcon, unsigned long dwVer, int cxDesired, int cyDesired, unsigned int uFlags)
    {
        m_handle = User32::CreateIconFromResourceEx(pbIconBits, cbIconBits, fIcon ? 1:0, dwVer, cxDesired, cyDesired, uFlags);
        m_owning = true;
    }

    Icon::Icon(IconInfo& iconinfo)
    {
        m_handle = User32::CreateIconIndirect(reinterpret_cast<const ::ICONINFO*>(&iconinfo));
        m_owning = true;
    }

    Icon::Icon(void* hInstance, std::string lpIconName)
    {
        m_handle = User32::LoadIconT(reinterpret_cast<HINSTANCE>(hInstance), to_tstring(lpIconName.c_str()).c_str());
        m_owning = true;
    }

    Icon::Icon(const wchar_t* id)
    {
        m_handle = User32::LoadIconT(reinterpret_cast<HINSTANCE>(nullptr), id);
        m_owning = true;
    }

    Icon::~Icon()
    {
        if(m_owning)
        {
            User32::DestroyIcon(reinterpret_cast<HICON>(m_handle.Use()));
        }
    }

    const PrimitiveHandle& Icon::GetHandle() const
    {
        return m_handle;
    }

    Icon Icon::Clone()
    {
        PrimitiveHandle h(User32::CopyIcon(reinterpret_cast<HICON>(m_handle.Use())));
        return Icon(h, true);
    }

    bool Icon::Draw(void* hDC, int X, int Y)
    {
        return User32::DrawIcon(reinterpret_cast<HDC>(hDC), X, Y, reinterpret_cast<HICON>(m_handle.Use())) ? true : false;
    }

    bool Icon::DrawEx(void* hdc, int xLeft, int yTop, void* hIcon, int cxWidth, int cyWidth, unsigned int istepIfAniCur, const Handle& hbrFlickerFreeDraw, unsigned int diFlags)
    {
        return User32::DrawIconEx(reinterpret_cast<HDC>(hdc), xLeft, yTop, reinterpret_cast<HICON>(m_handle.Use()), cxWidth, cyWidth, istepIfAniCur, reinterpret_cast<HBRUSH>(hbrFlickerFreeDraw.Use()), diFlags) ? true : false;
    }

    bool Icon::GetInfo(IconInfo& iconinfo)
    {
        return User32::GetIconInfo(reinterpret_cast<HICON>(m_handle.Use()), reinterpret_cast<PICONINFO>(&iconinfo)) ? true : false;
    }

    bool Icon::GetInfoExT(IconInfoEx& iconinfoex)
    {
        return User32::GetIconInfoExT(reinterpret_cast<HICON>(m_handle.Use()), reinterpret_cast<PICONINFOEX>(&iconinfoex)) ? true : false;
    }

    int Icon::LookupIdFromDirectory(byte* presbits, bool fIcon)
    {
        return User32::LookupIconIdFromDirectory(presbits, fIcon ? 1 : 0);
    }

    int Icon::LookupIdFromDirectoryEx(byte* presbits, bool fIcon, int cxDesired, int cyDesired, unsigned int Flags)
    {
        return User32::LookupIconIdFromDirectoryEx(presbits, fIcon ? 1 : 0, cxDesired, cyDesired, Flags);
    }

    unsigned int Icon::PrivateExtractT(const std::string& lpszFile, int nIconIndex, int cxIcon, int cyIcon, void** phicon, unsigned int* piconid, unsigned int nIcons, unsigned int flags)
    {
        return User32::PrivateExtractIconsT(to_tstring(lpszFile.c_str()).c_str(), nIconIndex, cxIcon, cyIcon, reinterpret_cast<HICON*>(phicon), piconid, nIcons, flags);
    }

}}}//qor::platform::win
