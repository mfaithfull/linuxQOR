// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_CODEPAGE_UTF8
#define QOR_PP_H_TEXT_CODEPAGE_UTF8

#include "../codepage.h"

namespace qor{

	class qor_pp_module_interface_gcc(QOR_TEXT) UTF8CodePage : public CodePage< char8_t, Mib::UTF8 >
	{
	public:

		qor_pp_module_interface(QOR_TEXT) UTF8CodePage();
		UTF8CodePage& operator = (const UTF8CodePage&) = delete;
		qor_pp_module_interface(QOR_TEXT) virtual ~UTF8CodePage();

		qor_pp_module_interface(QOR_TEXT) virtual bool Encode(const CodePoint & codePoint, char8_t*& space, size_t& available) const override;
		qor_pp_module_interface(QOR_TEXT) virtual CodePoint Decode(const char8_t*& chars, size_t& available) const override;
    };

	template<>
	struct charset_of<Mib::UTF8>
	{
		typedef UTF8CodePage type;
	};

}//qor

#endif//QOR_PP_H_TEXT_CODEPAGE_UTF8
