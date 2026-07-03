// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "c.h"

namespace qor
{
	CCodePage::CCodePage() : CodePage("'C'", "'C'"){ }
	
	bool CCodePage::Encode(const CodePoint & codePoint, char*& space, size_t& available) const
	{
		if(available == 0 || space == nullptr)
		{
			return false;
		}
		char c = codePoint.Value() > 0x7F ? '?' : static_cast<char>(codePoint.UChar());
		*space++ = c;
		--available;
		return true;
	}

	CodePoint CCodePage::Decode(const char*& chars, size_t& available) const
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
