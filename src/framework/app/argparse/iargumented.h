// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ARGPARSE_IARGUMENTED
#define QOR_PP_H_ARGPARSE_IARGUMENTED

#include <string>
#include "arguments.h"

namespace qor{ namespace app{

    struct OptConfig
    {
        bool allowAbbreviation{true};
        bool addHelp{true};
        bool ignoreUnknownArgs{true};
        char prefixChars{'-'};
    };
    
    class IArgumented
	{
	public:
	
        virtual const std::string Name() = 0;
        virtual const std::string Description() = 0;
        virtual const std::string UsageEpilogue() = 0;
        virtual const std::string OverrideUsage() = 0;
        virtual const OptConfig Config() = 0;
		virtual const std::vector<NamedArgSpec> NamedArguments() = 0;
		virtual const std::vector<PositionalArgSpec> PositionalArguments() = 0;
	};

}}//qor::app

#endif//QOR_PP_H_ARGPARSE_IARGUMENTED
