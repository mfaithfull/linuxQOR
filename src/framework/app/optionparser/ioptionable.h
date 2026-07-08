// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OPTIONPARSER_INTERFACE
#define QOR_PP_H_OPTIONPARSER_INTERFACE

#include "option.h"

namespace qor{ namespace app{

    //Implement this interface to be able to get command line options passed to your application
    class IOptionable
	{
	public:
		
		virtual const char* ProvideShortOptionString() = 0;
		virtual Option* ProvideLongOptions() = 0;
		virtual void ReceiveOptionSwitch(char c) = 0;
		virtual void ReceiveOptionParameter(char c, const char*) = 0;
		virtual void ReceiveLongOption(const char* option, const char* value) = 0;
		virtual void ReceiveNonOption(const char* parameter) = 0;
	};

}}//qor::app

#endif//QOR_PP_H_OPTIONPARSER_INTERFACE
