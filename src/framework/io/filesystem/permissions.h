// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_FILESYSTEM_PERMISSIONS
#define QOR_PP_H_PLATFORM_FILESYSTEM_PERMISSIONS

#include <filesystem>

namespace qor{ namespace io{ namespace filesystem{

    enum class Permissions : unsigned {
        None		=  static_cast<unsigned int>(std::filesystem::perms::none),
        OwnerRead	=  static_cast<unsigned int>(std::filesystem::perms::owner_read),
        OwnerWrite	=  static_cast<unsigned int>(std::filesystem::perms::owner_write),
        OwnerExec	=  static_cast<unsigned int>(std::filesystem::perms::owner_exec),
        OwnerAll	=  static_cast<unsigned int>(std::filesystem::perms::owner_all),
        GroupRead	=  static_cast<unsigned int>(std::filesystem::perms::group_read),
        GroupWrite	=  static_cast<unsigned int>(std::filesystem::perms::group_write),
        GroupExec	=  static_cast<unsigned int>(std::filesystem::perms::group_exec),
        GroupAll	=  static_cast<unsigned int>(std::filesystem::perms::group_all),
        OthersRead	=  static_cast<unsigned int>(std::filesystem::perms::others_read),
        OthersWrite =  static_cast<unsigned int>(std::filesystem::perms::others_write),
        OthersExec	=  static_cast<unsigned int>(std::filesystem::perms::others_exec),
        OthersAll	=  static_cast<unsigned int>(std::filesystem::perms::others_all),
        All		    =  static_cast<unsigned int>(std::filesystem::perms::all),
        Set_uid		=  static_cast<unsigned int>(std::filesystem::perms::set_uid),
        Set_gid		=  static_cast<unsigned int>(std::filesystem::perms::set_gid),
        Sticky_bit	=  static_cast<unsigned int>(std::filesystem::perms::sticky_bit),
        Mask		=  static_cast<unsigned int>(std::filesystem::perms::mask),
        Unknown		=  static_cast<unsigned int>(std::filesystem::perms::unknown),
    };

    constexpr Permissions
    operator&(Permissions __x, Permissions __y) noexcept
    {
        using __utype = typename std::underlying_type<Permissions>::type;
        return static_cast<Permissions>(
        static_cast<__utype>(__x) & static_cast<__utype>(__y));
    }

    constexpr Permissions
    operator|(Permissions __x, Permissions __y) noexcept
    {
        using __utype = typename std::underlying_type<Permissions>::type;
        return static_cast<Permissions>(
        static_cast<__utype>(__x) | static_cast<__utype>(__y));
    }

    constexpr Permissions
    operator^(Permissions __x, Permissions __y) noexcept
    {
        using __utype = typename std::underlying_type<Permissions>::type;
        return static_cast<Permissions>(
        static_cast<__utype>(__x) ^ static_cast<__utype>(__y));
    }

    constexpr Permissions
    operator~(Permissions __x) noexcept
    {
        using __utype = typename std::underlying_type<Permissions>::type;
        return static_cast<Permissions>(~static_cast<__utype>(__x));
    }

    inline Permissions&
    operator&=(Permissions& __x, Permissions __y) noexcept
    { return __x = __x & __y; }

    inline Permissions&
    operator|=(Permissions& __x, Permissions __y) noexcept
    { return __x = __x | __y; }

    inline Permissions&
    operator^=(Permissions& __x, Permissions __y) noexcept
    { return __x = __x ^ __y; }
        
    //bitmask for changes to permissions
    enum class PermissionOptions : unsigned {
        Replace     = static_cast<unsigned>(std::filesystem::perm_options::replace),
        Add         = static_cast<unsigned>(std::filesystem::perm_options::add),
        Remove      = static_cast<unsigned>(std::filesystem::perm_options::remove),
        NoFollow    = static_cast<unsigned>(std::filesystem::perm_options::nofollow)
    };

}}}//filesystem::io::qor

#endif//QOR_PP_H_PLATFORM_FILESYSTEM_PERMISSIONS