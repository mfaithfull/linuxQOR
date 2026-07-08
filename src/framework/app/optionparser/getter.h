// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OPTIONPARSER_GETTER
#define QOR_PP_H_OPTIONPARSER_GETTER

#include <stdlib.h>

#include "ioptionable.h"
#include "shortoption_interpretter.h"
#include "context.h"
#include "nonopt_tracker.h"
#include "longoption_interpretter.h"

#include <string.h>

namespace qor{ namespace app{

	class qor_pp_module_interface(QOR_OPTIONPARSER) OptionGetter
	{
	public:

		OptionGetter(const int argc, const char** argv);
		OptionGetter(const int argc, const char** argv, const char* shortOptions, Option* longOptions);
		OptionGetter(const int argc, const char** argv, IOptionable& optionable, bool longOnly = false, bool posixCompliant = false, int option_index = 0);
		void SetOptions(const char* shortOptions, Option * longOptions);
		void GetOptions(IOptionable& optionable, bool longOnly = false, bool posixCompliant = false, int option_index = 0);

	private:

        arg_iter itArgument;
        NonOptTracker nonOpts;
        ShortOptInterpretter shortOptionInterpretter;
        LongOptInterpretter longOptioninterpretter;
		Context m_optionsContext;

		bool ParseNextOption(int& result);
		bool HandleEndOfOptions(int& result);
		bool HandleNonOption(int& result);
		bool HandleLongOption(int& result);
		char ParseNextShortOption();
		int ParseSpecialWCase(char& c);
		void ParseArgumentCase(char& c, const char* temp);
		void ParseRequiredArgumentCase(char& c);
		void ParseOptionalArgumentCase();
		int Internal(bool longOnly, bool posixlyCorrect);

	};

}}//qor::app

#endif//QOR_PP_H_OPTIONPARSER_GETTER
