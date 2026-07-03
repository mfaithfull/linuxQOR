// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#ifdef NDEBUG
#include <buildnumber.h>
#endif
#include "src/qor/module/module.h"
#include "codepage/codepages.h"
#include "codepage/codepageregentry.h"

extern "C"
{
	qor::Module& ThisModule(void)
	{
		static qor::Module QORModule("Querysoft Open Runtime: Text Module", qor_pp_module_ver_string );

		static qor::CCodePage sCCodePage;
		static qor::ASCIICodePage sASCIICodePage;
		static qor::UTF16CodePage sUTF16CodePage;
		static qor::UTF32CodePage sUTF32CodePage;
		static qor::ISOLatin1CodePage sISOLatin1CodePage;
		static qor::UTF8CodePage sUTF8CodePage;
		static qor::ISOLatin2CodePage sISOLatin2CodePage;
		static qor::ISO8859_3CodePage sISO8859_3CodePage;
		static qor::ISO8859_4CodePage sISO8859_4CodePage;

		static qor::CodePageRegEntry< char, qor::Mib::C > regCCodePage(&sCCodePage);
		static qor::CodePageRegEntry< char, qor::Mib::ASCII > regASCIICodePage(&sASCIICodePage);
		static qor::CodePageRegEntry< char16_t, qor::Mib::UTF16 > regUTF16CodePage(&sUTF16CodePage);
		static qor::CodePageRegEntry< char32_t, qor::Mib::UTF32 > regUTF32CodePage(&sUTF32CodePage);
		static qor::CodePageRegEntry< char8_t, qor::Mib::ISOLatin1 > regISOLatin1CodePage(&sISOLatin1CodePage);
		static qor::CodePageRegEntry< char8_t, qor::Mib::UTF8 > regUTF8CodePage(&sUTF8CodePage);
		static qor::CodePageRegEntry< char8_t, qor::Mib::ISOLatin2> regISOLatin2CodePage(&sISOLatin2CodePage);
		static qor::CodePageRegEntry< char8_t, qor::Mib::ISOLatin3> regISO8859_3CodePage(&sISO8859_3CodePage);
		static qor::CodePageRegEntry< char8_t, qor::Mib::ISOLatin4> regISO8859_4CodePage(&sISO8859_4CodePage);
		
		return QORModule;
	}
}
