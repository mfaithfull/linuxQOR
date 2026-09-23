// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_MIXINS_COMPOSITION
#define QOR_PP_H_FRAMEWORK_MIXINS_COMPOSITION

#include <tuple>
#include "foreach.h"
#include "conditional.h"

namespace qor{ namespace mix{

    //results in the dynamic input. Use to break recursive chains or forward args /
    // identity is one of the core concepts of cmpl. It is used as the default continuation and
    // when reached it breaks the chain as it has not continuation it self. it also comes in
    // handy as a place holder in each if one only wants to modify some of the arguments. It is
    // also used as a default predicate in algorithms like all where a predicate may be
    // unneeded.
    struct identity 
    {
        template <typename T>
        using f = T;
    };

    namespace eager 
    {
        template <typename T>
        using identity = T;
    } 

	namespace detail {

        template <bool b, typename C>
		struct dependant_impl;

		template <typename C>
		struct dependant_impl<true, C> : C {};

	}//detail

	template <typename C, unsigned size>
	using dcall = typename detail::dependant_impl<static_cast<bool>(size < 100000), C>;

	template <bool>
	struct dcallf;

	template <>
	struct dcallf<true> 
    {
		template <template <typename...> class F1, typename... Ts>
		using f1 = F1<Ts...>;
		
        template <template <typename...> class F1, template <typename...> class F2, typename... Ts>
		using f2 = F1<F2<Ts...>>;
	};

    template <>
    struct dcallf<false> 
    {
        template <template <typename...> class F1, typename... Ts>
        using f1 = F1<>;

        template <template <typename...> class F1, template <typename...> class F2, typename... Ts>
        using f2 = F1<F2<>>;
    };

	namespace detail 
    {
		template <typename C, typename L, typename... Ts>
		struct unpack_impl;

		template <typename C, template <typename...> class Seq, typename... Ls, typename... Ts>
		struct unpack_impl<C, Seq<Ls...>, Ts...> 
        {
			using type = typename dcall<C, sizeof...(Ls)>::template f<Ts..., Ls...>;
		};
	}//detail

	//Turns a list of types into a variadic pack of those types /
	// example: call<all<>,ture_,false_,true_> is equivalent to
	// call<unpack<all<>>,list<true_,false_,true_>>
	template <typename C>
	struct unpack 
    {
		template <typename... Ls>
		using f = typename detail::unpack_impl<C, Ls...>::type;
	};

	// call a continuation (left parameter) with a variadic pack
	template <typename C, typename... Ts>
	using call = typename dcall<C, sizeof...(Ts)>::template f<Ts...>;

	template <typename C = identity>
	struct call_f 
    {
		template <typename... Ts>
		using f = typename C::template f< dcallf<(sizeof...(Ts) < 10000)>::template f1<call, Ts...> >;
	};

	template <>
	struct call_f<identity> 
    {
		template <typename... Ts>
		using f = typename dcallf<(sizeof...(Ts) < 10000)>::template f1<call, Ts...>;
	};

    template< typename... Ts >
    class composition : public std::tuple< Ts... >
    {
        std::tuple< call< Ts, Ts... >... > data;
        friend access< composition< Ts... > >;
    };
    
    /*
    template< typename... Ts>
    class composition : public call_ < detail::make_base< composition< Ts...> >, Ts... >
    {
        //tuple of implementation mixins
        std::tuple< Ts... > data;
        //std::tuple< call< Ts, Ts... >... > data;
        friend access< composition< Ts... > >;
    public:
        composition(std:tuple< Ts... >&& d) : data{std::move(d)}
        {
            for_each(this, ability<requires_init_and_destruct>, 
                detail::call_init(this));
        }
        ~composition()
        {
            for_each(this, ability<requries_init_and_destruct>,
                detail::call_destruct(this));
        }
    };*/

    //auto thing = compose(interface<bells, whistles>, 
    //  guts{haggis}, more_guts, my_allocator{arena});
    //thing.ring
    /*
    using guts = make_mixin<
        guts_impl,//impl
        ringable,//ability
        magic_frog_power,//ability
        allocator_use_capable>;

        using guts = make_dynamic_mixin<
        fixed_buffer_factory,
        allocator
        >;
    */
}}//qor::mix

#endif//QOR_PP_H_FRAMEWORK_MIXINS_COMPOSITION