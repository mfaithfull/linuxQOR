// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/tdd/test/test.h"
#include "src/qor/module/module.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/platform/platform.h"
#include "src/framework/app/application/builder.h"
#include "src/framework/app/role/role.h"
#include "src/framework/app/argparse/argumentparser.h"

qor_pp_module_requires(ICurrentThread)

using namespace qor;
using namespace qor::app;
using namespace qor::platform;

class PseudoApplication : public app::IArgumented
{
    static const OptConfig config;
    static const std::vector<NamedArgSpec> namedArgs;
    static const std::vector<PositionalArgSpec> positionalArgs;

    virtual const std::string Name()
    {
        return std::string("PseudoApplication");
    }

    virtual const std::string Description()
    {
        return std::string("An argumented class like an Application.");
    }

    virtual const std::string UsageEpilogue()
    {
        return std::string("PseudoApplication implements IArgumented to allow it to receive arguments from an argument parser.");
    }

    virtual const std::string OverrideUsage()
    {
        return std::string();//Allow usage to be generated
    }

    virtual const OptConfig Config()
    {
        return config;
    }

    virtual const std::vector<NamedArgSpec> NamedArguments()
    {
        return namedArgs;
    }

    virtual const std::vector<PositionalArgSpec> PositionalArguments()
    {
        return positionalArgs;
    }

public:

    void ParseArgs(const int argc, const char** argv)
    {
        auto parser = ArgumentParser(*this);
        auto args = parser.ParseArgs(argc,argv);
        if(!args.IsArgValid())
        {            
            std::cout << parser.GetUsage() << std::endl;
            std::cout << args.GetErrorString() << std::endl;
        }
    }

};

const OptConfig PseudoApplication::config{
    .allowAbbreviation = true,
    .addHelp = true,
    .ignoreUnknownArgs = true,
    .prefixChars = '-'
};

const std::vector<NamedArgSpec> PseudoApplication::namedArgs = {{
    {
        "option",
        "",
        1,
        ArgTypeCast::e_String,
        false,
        "This should be an optional -option option."
    }
}};

const std::vector<PositionalArgSpec> PseudoApplication::positionalArgs = {{
    {
        "first",
        1,
        ArgTypeCast::e_String,
        true,
        "Whatever is passed first ends up here"
    }    
}};



int main(int argc, const char** argv, char** env)
{
	return AppBuilder().Build<Application>(
        "Test ArgumentParser",
        [argc,argv](ref_of<Application>::type app)
        {            
            PseudoApplication pseudoapp;
            pseudoapp.ParseArgs(argc, argv);
        }        
    )(qor_unlocked).SetRole<app::Role>(
		[](ref_of<app::IRole>::type role)
		{
		}
	).Run(
        [argv, argc]()->int
		{
			return qor::test::Factory::Instance().main(argc, argv);
		}
	);
}

extern "C"
{
	qor::Module& ThisModule(void)
	{
		static qor::Module QORModule("Querysoft Open Runtime: Test ArgParse Module", qor_pp_module_ver_string );
		return QORModule;
	}
}