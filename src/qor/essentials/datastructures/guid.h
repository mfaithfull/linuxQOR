// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_GUID
#define QOR_PP_H_GUID

#include <stdint.h>
#include <compare>

namespace qor
{
	struct GUID //A structure for globally unique identification
	{
		uint32_t Data1;
		uint16_t Data2;
		uint16_t Data3;
		uint8_t Data4[8];

        auto operator<=>(const GUID& cmp) const
        {
            if (Data1 != cmp.Data1) return Data1 <=> cmp.Data1;
            if (Data2 != cmp.Data2) return Data2 <=> cmp.Data2;
            if (Data3 != cmp.Data3) return Data3 <=> cmp.Data3;
            // Compare Data4 byte-by-byte
            for (int i = 0; i < 8; ++i) 
            {
                if (Data4[i] != cmp.Data4[i]) return Data4[i] <=> cmp.Data4[i];
            }
            return std::strong_ordering::equal;
        }

        bool operator ==(const GUID& cmp) const
        {
            return Data1 == cmp.Data1 && Data2 == cmp.Data2 && Data3 == cmp.Data3 && 
                Data4[0] == cmp.Data4[0] &&
                Data4[1] == cmp.Data4[1] &&
                Data4[2] == cmp.Data4[2] &&
                Data4[3] == cmp.Data4[3] &&
                Data4[4] == cmp.Data4[4] &&
                Data4[5] == cmp.Data4[5] &&
                Data4[6] == cmp.Data4[6] &&
                Data4[7] == cmp.Data4[7];
        }
    };

    constexpr GUID null_guid = {0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0}};

	typedef const GUID IID;

}//qor

#endif//QOR_PP_H_GUID
