// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_LOG_LEVEL
#define QOR_PP_H_LOG_LEVEL

#include <string_view>

namespace qor{ namespace log {

    enum class Level
    {
        Debug = 0,      //Everything but only in Debug builds
        Informative,    //Things that are done, normal flow, completions, opens, closes
        Important,      //Things that are unexpected or off the happy path, missing resources, exceptions, failures
        Impactful,      //Things that will change the flow of control and functional outcomes, exceptions, serious errors
        Imperative,     //Things that will cause a program to start or stop including Fatal errors
    };

    constexpr std::string_view LevelNames[] = {
        "Debug",
        "Informative",
        "Important",
        "Impactful",
        "Imperative"
    };

}}//qor::log

#endif//QOR_PP_H_LOG_LEVEL
