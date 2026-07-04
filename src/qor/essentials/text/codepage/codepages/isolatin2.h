// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_CODEPAGE_ISOLATIN2
#define QOR_PP_H_TEXT_CODEPAGE_ISOLATIN2

#include "../codepage.h"

namespace qor{

	class ISOLatin2CodePage : public CodePage< char8_t, Mib::ISOLatin2 >
	{
	public:		

		ISOLatin2CodePage();
		ISOLatin2CodePage& operator = (const ISOLatin2CodePage&) = delete;
		virtual ~ISOLatin2CodePage() = default;

		virtual bool Encode(const CodePoint & codePoint, char8_t*& space, size_t& available) const override;
		virtual CodePoint Decode(const char8_t*& chars, size_t& available) const override;
    };

	template<>
	struct charset_of<Mib::ISOLatin2>
	{
		typedef ISOLatin2CodePage type;
	};

}//qor

#endif//QOR_PP_H_TEXT_CODEPAGE_ISOLATIN1