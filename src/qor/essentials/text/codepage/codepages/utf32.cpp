// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "utf32.h"

namespace
{
	std::string aliases[] = {"csUTF32"};
}//

namespace qor
{
	UTF32CodePage::UTF32CodePage() : CodePage("UTF-32", "UTF-32", aliases, 1){ }
	
	bool UTF32CodePage::Encode(const CodePoint & codePoint, char32_t*& space, size_t& available) const
	{
		if(available == 0 || space == nullptr)
		{
			return false;
		}
		char32_t cp = codePoint.UChar();
		// Validate Unicode scalar value: 0x0–0x10FFFF excluding surrogates 0xD800–0xDFFF
		if(cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF))
		{
			cp = 0xFFFD; // replacement character
		}
    	*space++ = cp;
	    --available;		
		return true;    	
	}

	CodePoint UTF32CodePage::Decode(const char32_t*& chars, size_t& available) const
	{
        uint32_t cp = 0xFFFD;
		if (chars != nullptr && available > 0)
		{
			cp = static_cast<uint32_t>(*chars++);
			if(cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF))
			{
				cp = 0xFFFD; // replacement character
			}			
    		--available;
		}
		return CodePoint(cp);
	}

}//qor
