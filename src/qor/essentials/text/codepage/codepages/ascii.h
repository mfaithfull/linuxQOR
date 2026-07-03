// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_CODEPAGE_ASCII
#define QOR_PP_H_TEXT_CODEPAGE_ASCII

#include "../codepage.h"

namespace qor{

	class qor_pp_module_interface(QOR_TEXT) ASCIICodePage : public CodePage< char, Mib::ASCII >
	{
	public:		

		ASCIICodePage();
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