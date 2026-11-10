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

    //U20 - the Icon half of the ownership model: it destroys on a flag set for a handle the OS owns.
    //(Note landed by the winqor_api_user review; its entry is src/platform/os/windows/api_layer/user/registers/u20.md,
    //opened 30 Oct 2026 (session 9, cursor.cpp) and closed 11 Nov 2026 by decision D47. The Icon half below was
    //measured rather than inferred on 1 Nov 2026 (session 18, icon.cpp). The cites are that entry's, carried as
    //written - nothing in this file was re-audited for them.)
    //This destructor destroys on m_owning, and the two LoadIconT ctors above set that flag for a handle they do not
    //own: icon.cpp:46-50 (hInstance, name) and :52-56 (const wchar_t* id) both set m_owning = true, while the
    //identical Cursor ctors set it false (cursor.cpp:35-41, :49-53). One of the two is wrong for the same SDK call.
    //Measured against user32.dll: LoadIconW(NULL, IDI_APPLICATION) -> 0x1002B and a second load of the same id returns
    //the same value, so it is one shared handle, not a fresh one; DestroyIcon on it returns 1 with GetLastError 1402
    //(ERROR_INVALID_HANDLE), a second DestroyIcon on the same value also returns 1, and GetIconInfo on it still
    //succeeds afterwards - the predefined ids are not handle-table entries at all, the value sits in the low word.
    //The contract is violated and the OS absorbs it, which is why destroying a shared handle has never looked like a
    //failure here.
    //windowfactory.cpp:92's Icon(nullptr) reaches :52-56 and not the handle ctor - PrimitiveHandle(void*) at
    //common/handles/handle.h:14 is not explicit, but the standard conversion nullptr -> const wchar_t* outranks the
    //user-defined one - so this asks the OS for an icon with a null name: LoadIconW(NULL, nullptr) -> NULL with
    //GetLastError 1813 (ERROR_RESOURCE_NAME_NOT_FOUND), and this destructor then runs DestroyIcon(NULL) -> 0 with
    //GetLastError 1402. The "no small icon" intent is served only through a failure path, and the layer deliberately
    //reports no error on HICON returns (U2), so a caller cannot tell "no icon wanted" from "icon load failed" -
    //m_handle is null either way.
    //Checked and not a defect: the six ctors that assign m_owning inside the body (:22-56) cannot leave a half-built
    //object with an indeterminate flag, because if the wrapper call throws (Library::Call throws when the module or the
    //name is missing) the ctor never completes and ~Icon is never run. Recorded so the item is not re-opened.
    //Recorded, not fixed: m_owning is this class's ownership contract and the class is winqor_gui published surface -
    //changing who destroys a handle is a behaviour change in another library, which the api_layer review records
    //rather than performs. That is the rule U20 states, and U20 closes on it at D47 with these notes in place.


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
