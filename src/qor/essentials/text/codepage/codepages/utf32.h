// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_CODEPAGE_UTF32
#define QOR_PP_H_TEXT_CODEPAGE_UTF32

#include "../codepage.h"

namespace qor{

	class qor_pp_module_interface(QOR_TEXT) UTF32CodePage : public CodePage< char32_t, Mib::UTF32 >
	{
	public:

		UTF32CodePage();
		virtual ~UTF32CodePage() = default;

		virtual bool Encode(const CodePoint & codePoint, char32_t*& space, size_t& available) const override;
		virtual CodePoint Decode(const char32_t*& chars, size_t& available) const override;
    };

	template<>
	struct charset_of<Mib::UTF32>
	{
		typedef UTF32CodePage type;
	};

}//qor

#endif//QOR_PP_H_TEXT_CODEPAGE_UTF32