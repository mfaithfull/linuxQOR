// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_CODEPAGE_ASCII
#define QOR_PP_H_TEXT_CODEPAGE_ASCII

#include "../codepage.h"

namespace qor{

	class ASCIICodePage : public CodePage< char, Mib::ASCII >
	{
	public:		

		ASCIICodePage();
		ASCIICodePage(const ASCIICodePage&) = delete;
		ASCIICodePage& operator = (const ASCIICodePage&) = delete;
		virtual ~ASCIICodePage() = default;

		virtual bool Encode(const CodePoint & codePoint, char*& space, size_t& available) const override;
		virtual CodePoint Decode(const char*& chars, size_t& available) const override;
    };

	template<>
	struct charset_of<Mib::ASCII>
	{
		typedef ASCIICodePage type;
	};

}//qor

#endif//QOR_PP_H_TEXT_CODEPAGE_ASCII