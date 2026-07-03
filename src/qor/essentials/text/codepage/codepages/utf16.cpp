// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "utf16.h"

namespace
{
	std::string aliases[] = {"csUTF16"};
}

namespace qor
{
	UTF16CodePage::UTF16CodePage() : CodePage("UTF-16", "UTF-16", aliases, 1){ }
	
	bool UTF16CodePage::Encode(const CodePoint & codePoint, char16_t*& space, size_t& available) const
	{
		if(available < 1 || space == nullptr)
		{
			return false;
		}

		char16_t c = codePoint.Value() > 0xFFFF ? 0xFFFD : static_cast<char16_t>(codePoint.UChar());

		if (c >= 0xD800 && c <= 0xDFFF)
		{
			char32_t twentybit = codePoint.UChar() - 0x10000;
			char16_t highSurrogate = 0xD800 + ((twentybit >> 10) & 0x03FF);
			char16_t lowSurrogate = 0xDC00 + (twentybit & 0x3FF);
			*space++ = highSurrogate;
			*space++ = lowSurrogate;
			available -= 2;
		}
		else
		{
			*space++ = c;
			--available;
		}
		return true;    	
	}

	CodePoint UTF16CodePage::Decode(const char16_t*& chars, size_t& available) const
	{
		uint32_t cp = 0xFFFD;// Replacement character for invalid input
		if (chars != nullptr && available > 0)
		{
			char16_t c = *chars;

			if (c >= 0xD800 && c <= 0xDFFF)
			{
				char32_t highDecode = ((c - 0xD800) << 10);
				available --;
				if(available > 0)
				{
					c = *chars++;
					char32_t lowDecode = (c - 0XDC00);
					cp = static_cast<uint32_t>(highDecode + lowDecode);	
				}
				else
				{
					//unpaired surrogate. Windows sometimes does this so allow it
					cp = highDecode;
				}
			}
			else
			{
				cp = static_cast<uint32_t>(c);
				available --;
			}
		}
		return CodePoint(cp);
	}

}//qor
