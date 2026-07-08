// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "compiler.h"

#ifdef qor_pp_intrinsics_header
#endif

namespace qor{ namespace compiler {

    const char* Compiler::Name()
    {
        return qor_pp_compiler_name;
    }

    static const Compiler theCompiler;

    const Compiler* TheCompiler()
    {
        return &theCompiler;
    }

}}//qor::compiler