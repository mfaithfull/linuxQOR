// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_CODEPAGE_C
#define QOR_PP_H_TEXT_CODEPAGE_C

#include "../codepage.h"

namespace qor{

	class CCodePage : public CodePage< char, Mib::C >
	{
	public:		

		CCodePage();
		CCodePage(const CCodePage&) = delete;
		CCodePage& operator = (const CCodePage&) = delete;
		virtual ~CCodePage() = default;

		virtual bool Encode(const CodePoint & codePoint, char*& space, size_t& available) const override;
		virtual CodePoint Decode(const char*& chars, size_t& available) const override;
    };

	template<>
	struct charset_of<Mib::C>
	{
		typedef CCodePage type;
	};

}//qor

#endif//QOR_PP_H_TEXT_CODEPAGE_C