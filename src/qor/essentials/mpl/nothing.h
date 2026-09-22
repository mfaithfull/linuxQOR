// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_NOTHING
#define QOR_PP_H_MPL_NOTHING

namespace qor { namespace mpl {
    /// class used when a function errors, as in the haskell nothing
    /// all functions that take nothing as a parameter should return nothing
    struct nothing { };
}}//qor::mpl

#endif//QOR_PP_H_MPL_NOTHING
