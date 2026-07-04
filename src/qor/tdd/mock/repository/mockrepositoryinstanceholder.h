// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_REPOINSTANCEHOLDER
#define QOR_PP_H_TESTMOCK_REPOINSTANCEHOLDER

#include <iostream>

namespace qor{ namespace mock{

    class MockRepository;
    class Reporter;

    inline std::ostream& operator<<(std::ostream& os, const MockRepository& repo);

    template <int X>
    class MockRepoInstanceHolder
    {
    public:
        static MockRepository* instance;
        static Reporter* reporter;
    };

    template <int X>
    MockRepository* MockRepoInstanceHolder<X>::instance;

    template <int X>
    Reporter* MockRepoInstanceHolder<X>::reporter;

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_REPOINSTANCEHOLDER
