// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


#include "src/configuration/configuration.h"

#include "getter.h"

using namespace qor;

namespace qor{ namespace components{ namespace optparser {

	OptionGetter::OptionGetter(const int argc, char** argv) : nonOpts(itArgument), 
    longOptioninterpretter(m_pArgument, m_pNextChar, shortOptionInterpretter, itArgument, m_RaiseErrors, m_OptionOption)
	{
        itArgument.Init(argc,argv);
		m_pArgument = nullptr;
		m_Error = 1;
		m_OptionOption = '?';
		Initialized = false;
		m_pNextChar = nullptr;
		m_Ordering = REQUIRE_ORDER;
		m_RaiseErrors = true;
	}

	void OptionGetter::SetOptions(const char* shortOptions, Option* longOptions)
	{
        shortOptionInterpretter.Init(shortOptions);
        longOptioninterpretter.Init(longOptions);
	}

	void OptionGetter::GetOptions(IOptionable& optionable, bool longOnly, bool posixCompliant, int option_index)
	{
        longOptioninterpretter.SetIndex(option_index);		
		int c;
		while ((c = Internal(longOnly, posixCompliant)) != -1)
		{
			(c == 0) ? optionable.ReceiveLongOption(longOptioninterpretter.GetName(), m_pArgument) :
				(m_pArgument ? optionable.ReceiveOptionParameter(c, m_pArgument) : optionable.ReceiveOptionSwitch(c));
		}
		while (itArgument.NotAtEnd())
		{
			optionable.ReceiveNonOption(itArgument.NextArg());
		}
	}

	int OptionGetter::Internal(bool longOnly, bool posixlyCorrect)
	{
		int result;
        longOptioninterpretter.SetLongOnly(longOnly);

		if (itArgument.IsEmpty())
		{
			result = -1;
		}
		m_pArgument = nullptr;
		if (itArgument.Index() == 0 || !Initialized)
		{
            nonOpts.Init();
			m_pNextChar = nullptr;
			Initialized = true;
			m_Ordering = shortOptionInterpretter.DetermineOptionOrdering(posixlyCorrect);
		}
		else if (shortOptionInterpretter.DashOrPlus())
		{
            shortOptionInterpretter.Next();
		}
		m_RaiseErrors = shortOptionInterpretter.Colon() ? 0 : m_Error;

		if (!((m_pNextChar == nullptr || *m_pNextChar == '\0') && ParseNextOption(result)))
		{
			result = ParseNextShortOption();
		}

		return result;
	}

	bool OptionGetter::ParseNextOption(int& result)
	{
		nonOpts.ParseNextOption();

		if (m_Ordering == PERMUTE)
		{
            nonOpts.Permute();
		}
		// The special ARGV-element '--' means premature end of options.
		//Skip it like a null option, then exchange with previous non-options as if it were an option, then skip everything else like a non-option.
		if (itArgument.IsDoubleDash())
		{
            nonOpts.HandleDoubleDash();
		}

		if (
			itArgument.AtEnd() && HandleEndOfOptions(result) || // If we have done all the ARGV-elements, stop the scan and back over any non-options that we skipped and permuted.
			(itArgument.AtNonOption() && HandleNonOption(result)) || // If we have come to a non-option and did not permute it, either stop the scan or describe it to the caller and pass it by.
			(longOptioninterpretter.HasLongOptions() && HandleLongOption(result))
			)// We have found another option-ARGV-element. Check whether it might be a long option.
		{
			return true;
		}
		else
		{
			// It is not a long option.  Skip the initial punctuation.
			m_pNextChar = itArgument.Arg() + 1;
			return false;
		}
	}

	bool OptionGetter::HandleEndOfOptions(int& result)
	{
        nonOpts.HandleEndOfOptions();
		result = -1;
		return true;
	}

	bool OptionGetter::HandleNonOption(int& result)
	{
		if (m_Ordering == REQUIRE_ORDER)
		{
			result = -1;
		}
		else
		{
			m_pArgument = itArgument.NextArg();
			result = 1;
		}
		return true;
	}

	bool OptionGetter::HandleLongOption(int& result)
	{
		if (itArgument.AtLongOption())
		{
			// "--foo" is always a long option.  The special option "--" was handled above.
			m_pNextChar = itArgument.Arg() + 2;
			result = longOptioninterpretter.ProcessLongOption("--");
			return true;
		}
		/* If m_LongOnly and the ARGV-element has the form "-f",
		   where f is a valid short option, don't consider it an
		   abbreviated form of a long option that starts with f.
		   Otherwise there would be no way to give the -f short
		   option.
		   On the other hand, if there's a long option "fubar" and
		   the ARGV-element is "-fu", do consider that an
		   abbreviation of the long option, just like "--fu", and
		   not "-f" with arg "u".
		   This distinction seems to be the most useful approach.  */
		if (longOptioninterpretter.IsLongOnly() && (itArgument.Arg()[2] || ! shortOptionInterpretter.Find(itArgument.Arg()[1])))
		{
			int code;
			m_pNextChar = itArgument.Arg() + 1;
			code = longOptioninterpretter.ProcessLongOption("-");
			if (code != -1)
			{
				result = code;
				return true;
			}
		}
		return false;
	}

	char OptionGetter::ParseNextShortOption()
	{
		// Look at and handle the next short option-character.
		char c = *m_pNextChar++;
		const char* temp = shortOptionInterpretter.Find(c);
		// Increment 'm_Index' when we start to process its last character.
		if (*m_pNextChar == '\0')
		{
            itArgument.Inc();
		}
		if (temp == nullptr || c == ':' || c == ';')
		{
			if (m_RaiseErrors)
			{
				//fprintf(stderr, _ATXT("%s: invalid option -- '%c'\n"), m_argv[0], c);
			}
			m_OptionOption = c;
			return '?';
		}
		// Convenience. Treat POSIX -W foo same as long option --foo
		if (temp[0] == 'W' && temp[1] == ';' && longOptioninterpretter.HasLongOptions())
		{
			ParseSpecialWCase(c);
		}
		if (temp[1] == ':')
		{
			ParseArgumentCase(c, temp);
		}
		return c;
	}

	int OptionGetter::ParseSpecialWCase(char& c)
	{
		// This is an option that requires an argument.
		if (*m_pNextChar != '\0')
		{
			m_pArgument = m_pNextChar;
		}
		else if (itArgument.AtEnd())
		{
			if (m_RaiseErrors)
			{
				//fprintf(stderr, _ATXT("%s: option requires an argument -- '%c'\n"), m_argv[0], c);
			}
			m_OptionOption = c;
			c = shortOptionInterpretter.ColonQuestion();// m_ShortOptionSet[0] == ':' ? ':' : '?';
			return c;
		}
		else
		{
			m_pArgument = itArgument.Arg();
		}
		m_pNextChar = m_pArgument;
		m_pArgument = nullptr;
		return longOptioninterpretter.ProcessLongOption("-W ");
	}

	void OptionGetter::ParseRequiredArgumentCase(char& c)
	{
		if (*m_pNextChar != '\0')
		{
			m_pArgument = m_pNextChar;// If we end this ARGV-element by taking the rest as an arg, we must advance to the next element now.
            itArgument.Next();
		}
		else if (itArgument.AtEnd())
		{
			if (m_RaiseErrors)
			{
				//fprintf(stderr, _ATXT("%s: option requires an argument -- '%c'\n"), m_argv[0], c);
			}
			m_OptionOption = c;
			c = shortOptionInterpretter.ColonQuestion();// m_ShortOptionSet[0] == ':' ? ':' : '?';
		}
		else
		{
			m_pArgument = itArgument.NextArg();
		}
		m_pNextChar = nullptr;
	}

	void OptionGetter::ParseOptionalArgumentCase()
	{
		if (*m_pNextChar != '\0')
		{
			m_pArgument = m_pNextChar;
            itArgument.Inc();
		}
		else
		{
			m_pArgument = nullptr;
		}
		m_pNextChar = nullptr;
	}

}}}//qor::components::optparser
