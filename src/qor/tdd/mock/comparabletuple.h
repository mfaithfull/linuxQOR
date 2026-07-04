// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_COMPARABLETUPLE
#define QOR_PP_H_TESTMOCK_COMPARABLETUPLE

#include <tuple>

namespace qor{ namespace mock{

    template <typename... Args>
    struct ComparableTupleBase
    {
    public:
        
        virtual ~ComparableTupleBase(){}

        virtual void print(std::ostream& os) const = 0;
        virtual void print(std::wostream& os) const = 0;
        virtual bool equals(const std::tuple<Args...>& rhs) = 0;
        virtual void assignInOut(std::tuple<Args...>& rhs) = 0;
    };

    template <typename tuple, typename... Args>
    struct ComparableTuple;

    template <typename... Args, typename... CArgs>
    struct ComparableTuple<std::tuple<Args...>, CArgs...> : public std::tuple<CArgs...>, public ComparableTupleBase<Args...>
    {
    public:

        ComparableTuple(CArgs... args) : std::tuple<CArgs...>(args...){}

        virtual ~ComparableTuple(){}

        void print(std::ostream& os) const override
        {
            printTuple(os, *this);
        }

        void print(std::wostream& os) const override
        {
            printTupleW(os, *this);
        }

        bool equals(const std::tuple<Args...>& rhs) override
        {
            // this effectively makes operator== virtual
            return (*this) == rhs;
        }

        void assignInOut(std::tuple<Args...>& rhs) override
        {
            assigner<0, sizeof...(Args), std::tuple<CArgs...>, std::tuple<Args...>>::Assign(*this, rhs);
        }
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_COMPARABLETUPLE
