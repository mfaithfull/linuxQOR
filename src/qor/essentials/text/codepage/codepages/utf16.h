// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_CODEPAGE_UTF16
#define QOR_PP_H_TEXT_CODEPAGE_UTF16

#include "../codepage.h"

namespace qor{

	class UTF16CodePage : public CodePage< char16_t, Mib::UTF16 >
	{
	public:

		UTF16CodePage();
		UTF16CodePage& operator = (const UTF16CodePage&) = delete;
		virtual ~UTF16CodePage() = default;

		virtual bool Encode(const CodePoint & codePoint, char16_t*& space, size_t& available) const override;
		virtual CodePoint Decode(const char16_t*& chars, size_t& available) const override;
    };

	template<>
	struct charset_of<Mib::UTF16>
	{
		typedef UTF16CodePage type;
	};

}//qor

#endif//QOR_PP_H_TEXT_CODEPAGE_UTF16
