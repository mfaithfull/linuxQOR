// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_APP_POSITIONALARG
#define QOR_PP_H_APP_POSITIONALARG

#include "argument.h"

namespace qor { namespace app {

    /// @brief Helper function to create positional argument
    /// @param positionalName Name of positional argument to access from code.
    /// @param argsCount Count of arguments
    /// (use kFromOneToInfiniteArgCount or kAnyArgCount for various arguments count)
    /// @param argType e_String, e_int, e_longlong, e_double, e_bool
    /// @param required Marker if argument should be passed or ignored if missed.
    /// @param help Initial part of help for current argument in case of auto-generated help.
    /// @return instance of Argument
    /// @note inline: see CreateNamedArgument -- required for multi-TU inclusion.
    inline Argument CreatePositionalArgument(const std::string& positionalName = "",
        const int argsCount = 1,
        ArgTypeCast argType = ArgTypeCast::e_String,
        const bool required = true,
        const std::string& help = "")
    {
        return Argument::CreatePositionalArgument(positionalName, argsCount, argType, required, help);
    }

    /// @brief Aggregate description of a positional argument, for keyword-style
    /// construction with C++20 designated initializers. See NamedArgSpec.
    struct PositionalArgSpec
    {
        std::string name = "";
        int nargs = 1;
        ArgTypeCast type = ArgTypeCast::e_String;
        bool required = true;
        std::string help = "";
    };

    /// @brief Keyword-style factory for a positional argument. See PositionalArgSpec.
    /// @param spec aggregate of the argument's properties
    /// @return instance of Argument
    inline Argument CreatePositionalArgument(const PositionalArgSpec& spec)
    {
        return Argument::CreatePositionalArgument(spec.name, spec.nargs,
            spec.type, spec.required, spec.help);
    }

}}//qor::app

#endif//QOR_PP_H_APP_POSITIONALARG