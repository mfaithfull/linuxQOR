// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_ISHELPERS
#define QOR_PP_H_FRAMEWORK_DIO_ISHELPERS

#include "src/qor/essentials/traits/typetraits.h"

#include "collectionadapter.h"

namespace qor{ namespace dio{

    template<typename x>
    constexpr bool is_valueadapter()
    {
        using x_type = std::remove_cvref_t<x>;
        return(
            is_instance_of_v<x_type, ValueAdapter> || 
            is_instance_of_v<x_type, ReadOnlyValueAdapter> ||
            is_instance_of_v<x_type, ReadWriteValueAdapter>
        );
    }

    template<typename x>
    constexpr bool is_streamoutable()
    {
        using x_type = std::remove_cvref_t<x>;    
        if constexpr(is_valueadapter<x>())
        {
            using v_type = typename x_type::value_type;
            return std::is_base_of_v<StreamOut<v_type>, x_type>;
        }
        else if constexpr(is_instance_of_v<x_type, CollectionAdapter>)
        {
            using c_type = typename x_type::collection_type;
            return std::is_base_of_v<StreamOutCollection< c_type >, x_type>;
        }
        return false;
    }

    template<typename x>
    constexpr bool is_stringable()
    {
        using x_type = std::remove_cvref_t<x>;
        if constexpr(is_valueadapter<x>())
        {
            using v_type = typename x_type::value_type;
            return std::is_base_of_v<ToString<v_type>, x_type>;
        }
        else if constexpr(is_instance_of_v< x_type, CollectionAdapter >)
        {
            using c_type = typename x_type::collection_type;
            return std::is_base_of_v< ToStringCollection< c_type >, x_type >;
        }
        return false;
    }

    template<typename x>
    constexpr bool is_readable()
    {
        using x_type = std::remove_cvref_t<x>;
        if constexpr(is_valueadapter<x>())
        {
            using v_type = typename x_type::value_type;
            return std::is_base_of_v<Read<v_type>, x_type>;
        }
        else if constexpr(is_instance_of_v< x_type, CollectionAdapter >)
        {
            using c_type = typename x_type::collection_type;
            return std::is_base_of_v< ReadCollection< c_type >, x_type >;
        }
        return false;
    }

    template<typename x>
    constexpr bool is_writeable()
    {
        using x_type = std::remove_cvref_t<x>;
        if constexpr(is_valueadapter<x>())
        {
            using v_type = typename x_type::value_type;
            return std::is_base_of_v<Write<v_type>, x_type> || std::is_base_of_v<EnhancedNumericWrite<v_type>, x_type>;
        }
        else if constexpr(is_instance_of_v< x_type, CollectionAdapter >)
        {
            using c_type = typename x_type::collection_type;
            return std::is_base_of_v< WriteCollection< c_type >, x_type >;
        }
        return false;
    }

    template<typename x>
    constexpr bool is_named()
    {
        using x_type = std::remove_cvref_t<x>;
        if constexpr(is_valueadapter<x>())
        {
            using v_type = typename x_type::value_type;
            return std::is_base_of_v<Name<v_type>, x_type>;
        }
        else if constexpr(is_instance_of_v< x_type, CollectionAdapter >)
        {
            using c_type = typename x_type::collection_type;
            return std::is_base_of_v< Name<c_type>, x_type >;
        }
        return false;
    }

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_ISHELPERS
