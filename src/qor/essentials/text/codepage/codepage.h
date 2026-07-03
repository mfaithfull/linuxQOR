// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0


#ifndef QOR_PP_H_TEXT_CODEPAGE_CODEPAGE
#define QOR_PP_H_TEXT_CODEPAGE_CODEPAGE

#include <vector>
#include <string>
#include "codepagebase.h"

namespace qor{

	template< typename C, Mib M = Mib::C >
	class CodePage : public CodePageBase< C, M >
	{
	public:

		typedef C charT;

		CodePage(const std::string& name, const std::string& MIMEName, const std::string* aliasArray = nullptr, uint32_t aliasCount = 0) : CodePageBase< C, M >(), m_name(name), m_MIMEName(MIMEName)
		{
			while (aliasArray != nullptr && aliasCount--)
			{
				std::string alias(aliasArray[aliasCount]);
				m_aliases.push_back(alias);
			}
		}

		virtual ~CodePage() = default;

		virtual inline bool Encode(const CodePoint & codePoint, C*& space, size_t& available) const = 0;
		virtual inline CodePoint Decode(const charT*& chars, size_t& available) const = 0;

	protected:

		std::string m_name;
		std::string m_MIMEName;
		std::vector< std::string > m_aliases;

    };

	//A trait to get base character unit type for Mib. Most will be char. UTF16 for example will be char16_t
	template<Mib M>
	struct charset_of
	{
		typedef CodePage<char, M> type;
	};

}//qor

#endif//QOR_PP_H_TEXT_CODEPAGE_CODEPAGE