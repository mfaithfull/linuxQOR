// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_CODEPAGE_UTF16
#define QOR_PP_H_TEXT_CODEPAGE_UTF16

#include "../codepage.h"

namespace qor{

	class qor_pp_module_interface_gcc(QOR_TEXT) UTF16CodePage : public CodePage< char16_t, Mib::UTF16 >
	{
	public:

		qor_pp_module_interface(QOR_TEXT) UTF16CodePage();
		UTF16CodePage& operator = (const UTF16CodePage&) = delete;
		qor_pp_module_interface(QOR_TEXT) virtual ~UTF16CodePage();

		qor_pp_module_interface(QOR_TEXT) virtual bool Encode(const CodePoint & codePoint, char16_t*& space, size_t& available) const override;
		qor_pp_module_interface(QOR_TEXT) virtual CodePoint Decode(const char16_t*& chars, size_t& available) const override;
    };

	template<>
	struct charset_of<Mib::UTF16>
	{
		typedef UTF16CodePage type;
	};

}//qor

#endif//QOR_PP_H_TEXT_CODEPAGE_UTF16
