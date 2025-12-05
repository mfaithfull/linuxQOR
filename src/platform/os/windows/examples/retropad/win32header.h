#ifndef QOR_PP_H_RETROPAD_WIN32
#define QOR_PP_H_RETROPAD_WIN32

struct ImageDOSHeader
{												// DOS .EXE header
    unsigned short e_magic;                     // Magic number
    unsigned short e_cblp;                      // Bytes on last page of file
    unsigned short e_cp;                        // Pages in file
    unsigned short e_crlc;                      // Relocations
    unsigned short e_cparhdr;                   // Size of header in paragraphs
    unsigned short e_minalloc;                  // Minimum extra paragraphs needed
    unsigned short e_maxalloc;                  // Maximum extra paragraphs needed
    unsigned short e_ss;                        // Initial (relative) SS value
    unsigned short e_sp;                        // Initial SP value
    unsigned short e_csum;                      // Checksum
    unsigned short e_ip;                        // Initial IP value
    unsigned short e_cs;                        // Initial (relative) CS value
    unsigned short e_lfarlc;                    // File address of relocation table
    unsigned short e_ovno;                      // Overlay number
    unsigned short e_res[4];                    // Reserved words
    unsigned short e_oemid;                     // OEM identifier (for e_oeminfo)
    unsigned short e_oeminfo;                   // OEM information; e_oemid specific
    unsigned short e_res2[10];                  // Reserved words
    long e_lfanew;								// File address of new exe header
};

#ifdef __MINGW32__
#	define __ImageBase _image_base__
#endif

	extern "C" const ImageDOSHeader __ImageBase;//This must be injected by the Compiler for Windows builds

#endif//QOR_PP_H_RETROPAD_WIN32