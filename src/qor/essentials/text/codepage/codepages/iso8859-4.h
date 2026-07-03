// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_CODEPAGE_I8859_4
#define QOR_PP_H_TEXT_CODEPAGE_I8859_4

#include "../codepage.h"

namespace qor{

	class qor_pp_module_interface(QOR_TEXT) ISO8859_4CodePage : public CodePage< char8_t, Mib::ISOLatin4 >
	{
	public:		

		ISO8859_4CodePage();
		virtual ~ISO8859_4CodePage() = default;

		virtual bool Encode(const CodePoint & codePoint, char8_t*& space, size_t& available) const override;
		virtual CodePoint Decode(const char8_t*& chars, size_t& available) const override;
    };

	template<>
	struct charset_of<Mib::ISOLatin4>
	{
		typedef ISO8859_4CodePage type;
	};

}//qor

#endif//QOR_PP_H_TEXT_CODEPAGE_I8859_4