// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCES_TTF_STRUCTURES
#define QOR_PP_H_FRAMEWORK_RESOURCES_TTF_STRUCTURES

#include <cstdint>

namespace qor{ namespace framework{ namespace res {

    struct offset_subtable
    {
        uint32_t	scaler_type; //0x74727565 ('true' as chars) or 0x00010000 for TTF   0x4F54544F ('OTTO' as chars) means its an OpenType with PostScript outlines  0x74797031 ('typ1' as chars) is type 1 postscript font files which was made by adobe in the 80s
        uint16_t	numTables;
        uint16_t	searchRange;
        uint16_t	entrySelector;
        uint16_t	rangeShift;
    };

    struct table_directory
    {
        uint32_t    tag;
        uint32_t	checkSum;
        uint32_t	offset;
        uint32_t	length;
    };

    struct cmap_header
    {
	    uint16_t version;
	    uint16_t numberSubtables;
    };


    struct font_directory
    {
        offset_subtable off_sub;
        table_directory* tbl_dir;
    };

}}}//qor::framework::res

#endif//QOR_PP_H_FRAMEWORK_RESOURCES_TTF_STRUCTURES
