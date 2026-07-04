// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_CODEPAGE_ISOLATIN1
#define QOR_PP_H_TEXT_CODEPAGE_ISOLATIN1

#include "../codepage.h"

namespace qor{

	class ISOLatin1CodePage : public CodePage< char8_t, Mib::ISOLatin1 >
	{
	public:		

		ISOLatin1CodePage();
		ISOLatin1CodePage& operator = (const ISOLatin1CodePage&) = delete;
		virtual ~ISOLatin1CodePage() = default;

		virtual bool Encode(const CodePoint & codePoint, char8_t*& space, size_t& available) const override;
		virtual CodePoint Decode(const char8_t*& chars, size_t& available) const override;
    };

	template<>
	struct charset_of<Mib::ISOLatin1>
	{
		typedef ISOLatin1CodePage type;
	};

}//qor

#endif//QOR_PP_H_TEXT_CODEPAGE_ISOLATIN1