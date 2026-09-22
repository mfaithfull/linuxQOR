// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEST_MPL_INSTANCE
#define QOR_PP_H_TEST_MPL_INSTANCE

#include "src/qor/essentials/mpl/list.h"
#include "src/qor/essentials/mpl/isinstance.h"

using namespace qor::mpl;
static_assert(eager::is_instance<list, list<void>>::value, "");
static_assert(!eager::is_instance<list, void>::value, "");

#endif//QOR_PP_H_TEST_MPL_INSTANCE
