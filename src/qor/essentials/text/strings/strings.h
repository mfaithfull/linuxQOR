// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_STRINGS_STRINGS
#define QOR_PP_H_TEXT_STRINGS_STRINGS

#include "../codepage/codepages.h"
#include "simplestring.h"

namespace qor{
    typedef SimpleString< char32_t > UCS4String;
    typedef SimpleString< char16_t > UCS2String;
    typedef SimpleString< char > ASCIIString;
    /*if you extend this set of specializations you also need to override 
    encoding_of 
    to declare a CodePage for the character type e.g.

    template<>
    struct encoding_of< char64_t >
    {
        typedef UTF64CodePage CodePageT;
    };
    typdef SimpleString< char64_t > UCS8String;
    */
}//qor

#include "localstring.h"

namespace qor{
    typedef LocalString< char8_t, Mib::ISOLatin1 > ISOLatin1String;
    typedef LocalString< char8_t, Mib::ISOLatin2 > ISOLatin2String;
    typedef LocalString< char8_t, Mib::ISOLatin3 > ISOLatin3String;
    typedef LocalString< char8_t, Mib::ISOLatin4 > ISOLatin4String;
    //Add types for additional code page support here
}//qor

#include "codestring.h"
#include "utf8string.h"
#include "utf16string.h"

namespace qor{
    typedef UTF16String WindowsString;
    typedef UTF8String String;    
    typedef typename String::CharT Char;
}

#endif//QOR_PP_H_TEXT_STRINGS_STRINGS
