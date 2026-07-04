// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_CODEPAGE_I8859_3
#define QOR_PP_H_TEXT_CODEPAGE_I8859_3

#include "../codepage.h"

namespace qor{

	class ISO8859_3CodePage : public CodePage< char8_t, Mib::ISOLatin3 >
	{
	public:		

		ISO8859_3CodePage();
		ISO8859_3CodePage(const ISO8859_3CodePage&) = delete;
		ISO8859_3CodePage& operator = (const ISO8859_3CodePage&) = delete;
		virtual ~ISO8859_3CodePage() = default;

		virtual bool Encode(const CodePoint & codePoint, char8_t*& space, size_t& available) const override;
		virtual CodePoint Decode(const char8_t*& chars, size_t& available) const override;
    };

	template<>
	struct charset_of<Mib::ISOLatin3>
	{
		typedef ISO8859_3CodePage type;
	};

}//qor

#endif//QOR_PP_H_TEXT_CODEPAGE_I8859_3