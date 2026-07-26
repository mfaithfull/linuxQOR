// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_APP_NAMEDARG
#define QOR_PP_H_APP_NAMEDARG

#include "argument.h"

namespace qor { namespace app {

    /// @brief Helper function to create named argument
    /// @param shortName Short name if needed. Will be used with single prefix.
    /// Short name could be auto-generated if possible when m_allowAbbrev in ParsedArgument set to true)
    /// @param longName Full name of argument
    /// @param argsCount Count of arguments
    /// (use ArgCountOneOrMore or ArgCountZeroOrMore for various arguments count)
    /// @param argType String, Int, LongLong, Double, Bool
    /// @param required Marker if argument should be passed or ignored if missed.
    /// @param help Initial part of help for current argument in case of auto-generated help.
    /// @return instance of Argument
    /// @note inline: this is a free function in a header, so it must have
    /// inline linkage to be safely included in more than one translation unit.
    inline Argument CreateNamedArgument(const std::string& shortName = "",
        const std::string& longName = "",
        const int argsCount = 1,
        ArgType argType = ArgType::String,
        const bool required = true,
        const std::string& help = "")
    {
        return Argument::CreateNamedArgument(shortName, longName, argsCount, argType, required, help);
    }

    /// @brief Aggregate description of a named argument, for keyword-style
    /// construction. Because it is a plain aggregate, C++20 designated
    /// initializers give a Python-like call site:
    /// @code
    ///   parser.AddArgument(argparse::CreateNamedArgument({
    ///       .longName = "numbers",
    ///       .nargs    = argparse::ArgCountOneOrMore,
    ///       .type     = argparse::ArgType::Int,
    ///       .required = false,
    ///       .help     = "some numbers"}));
    /// @endcode
    /// The same struct also works with ordinary aggregate init in C++11/14/17.
    struct NamedArgSpec
    {
        std::string shortName = "";
        std::string longName = "";
        int nargs = 1;
        ArgType type = ArgType::String;
        bool required = true;
        std::string help = "";
        void* target{nullptr};
    };

    /// @brief Keyword-style factory for a named argument. See NamedArgSpec.
    /// @param spec aggregate of the argument's properties
    /// @return instance of Argument
    inline Argument CreateNamedArgument(const NamedArgSpec& spec)
    {
        return Argument::CreateNamedArgument(spec.shortName, spec.longName,
            spec.nargs, spec.type, spec.required, spec.help, spec.target);
    }

}}//qor::app

#endif//QOR_PP_H_APP_NAMEDARG