// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

/*
	Copyright (C) 2017 by Sergey A Kryukov: derived work
	http://www.SAKryukov.org
	http://www.codeproject.com/Members/SAKryukov

	Based on original work by Sergey Ryazanov:
	"The Impossibly Fast C++ Delegates", 18 Jul 2005
	https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates

	MIT license:
	http://en.wikipedia.org/wiki/MIT_License

	Original publication: https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed

*/

#ifndef QOR_PP_H_DELEGATEHOLDER
#define QOR_PP_H_DELEGATEHOLDER

namespace qor { namespace detail {

    template< typename T >
    class DelegateHolder;

    template< typename ret_t, typename ...params_t >
    class DelegateHolder< ret_t(params_t...)>
    {
    protected:

        using stub_type = ret_t(*)(void* this_ptr, params_t...);

        struct InvocationElement
        {
            InvocationElement() = default;

            InvocationElement(void* this_ptr, stub_type aStub) : object(this_ptr), stub(aStub){}

            void Clone(InvocationElement& target) const
            {
                target.stub = stub;
                target.object = object;
            }

            bool operator == (const InvocationElement& another) const
            {
                return another.stub == stub && another.object == object;
            }

            bool operator != (const InvocationElement& another) const
            {
                return another.stub != stub || another.stub != object;
            }

            void* object = nullptr;
            stub_type stub = nullptr;

        };
    };

}}//qor::detail

#endif//QOR_PP_H_DELEGATEHOLDER
