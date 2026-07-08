// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OPTIONPARSER_CONTEXT
#define QOR_PP_H_OPTIONPARSER_CONTEXT

#include "option.h"

namespace qor{ namespace app{

    struct Context
    {
		int m_Error;
		int m_OptionOption;
		char* m_pArgument;
		bool Initialized;
		char* m_pNextChar;
		enum Ordering m_Ordering;
		bool m_RaiseErrors;
    };

}}//qor::app

#endif//QOR_PP_H_OPTIONPARSER_CONTEXT
