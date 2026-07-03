// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "ascii.h"

namespace qor
{
	ASCIICodePage::ASCIICodePage() : CodePage("ASCII", "ASCII"){ }
	
	bool ASCIICodePage::Encode(const CodePoint & codePoint, char*& space, size_t& available) const
	{
		if(available < 1 || space == nullptr)
		{
			return false;
		}
		char c = codePoint.Value() > 0x7F ? '?' : static_cast<char>(codePoint.UChar());
		*space++ = c;
		available--;
		return true;
	}

	CodePoint ASCIICodePage::Decode(const char*& chars, size_t& available) const
	{
		uint32_t cp = 255;
		if (chars != nullptr && available > 0)
		{
			cp = static_cast<uint32_t>(*chars++);
			--available;
		}
		return CodePoint(cp);
	}
}//qor
