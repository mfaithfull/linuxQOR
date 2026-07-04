// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_ARGUMENTPRINTER
#define QOR_PP_H_TESTMOCK_ARGUMENTPRINTER

#include <iostream>
#include <tuple>

namespace qor{ namespace mock{

    template<typename T>
    static inline std::ostream& output(std::ostream& os, unsigned int& arg, const T& t)
    {
        (++arg > 1) ? (os << ",") : (os << "");
        (os << t);            
        return os;
    }

    template<>
    inline std::ostream& output(std::ostream& os, unsigned int& arg, const wchar_t*const& t)
    {
        (++arg > 1) ? (os << ",") : (os << "");
        (os << (char)t[0] << (char)t[1]);            
        return os;
    }

    template <int index, int limit, typename Tuple>
    struct argumentPrinter
    {

        static void Print(std::ostream& os, const Tuple& t)
        {
            unsigned int arg = 0;
            std::apply([&os,&arg](auto&&... args) {                
                (output(os,arg,args), ...);
            }, t);
        }

        static void Print(std::wostream& os, const Tuple& t)
        {
            std::apply([&os](auto&&... args) {
                ((os << args << ","), ...);
            }, t);
        }

    };
    
    template <int limit, typename Tuple>
    struct argumentPrinter<limit, limit, Tuple>
    {
        static void Print(std::ostream&, const Tuple&)
        {
        }

        static void Print(std::wostream&, const Tuple&)
        {
        }
    };

    template <typename... Args>
    void printTuple(std::ostream& os, const std::tuple<Args...>& tuple)
    {
        os << "(";
        argumentPrinter<0, sizeof...(Args), std::tuple<Args...>>::Print(os, tuple);
        os << ")";
    }

    template <typename... Args>
    void printTupleW(std::wostream& os, const std::tuple<Args...>& tuple)
    {
        os << "(";
        argumentPrinter<0, sizeof...(Args), std::tuple<Args...>>::Print(os, tuple);
        os << ")";
    }

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_ARGUMENTPRINTER
