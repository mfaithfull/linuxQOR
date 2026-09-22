// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PARALLEL_WORKITEM_ID
#define QOR_PP_H_PARALLEL_WORKITEM_ID

#include <compare>
#include <cstdint>
#include <functional>
#include <limits>

#include "src/framework/parallel/signaltree/id.h"

namespace qor{ namespace par{

    // identifies an item within its owning work_item_group.  the value is
    // the signal index within that group's signal_set so conversion is free.
    class qor_pp_module_interface(QOR_WORKITEM) workitem_id
    {
    public:

        using value_type = uint64_t;

        constexpr workitem_id() noexcept = default;

        constexpr explicit workitem_id(value_type value) noexcept : value_(value) {}

        constexpr explicit workitem_id(signal_id id) noexcept :
            value_(static_cast<value_type>(id)) {}

        constexpr bool valid() const noexcept { return value_ != invalid_value; }

        constexpr explicit operator value_type() const noexcept { return value_; }

        constexpr signal_id to_signal_id() const noexcept { return signal_id{value_}; }

        constexpr auto operator <=>(workitem_id const &) const noexcept = default;

        static constexpr workitem_id invalid() noexcept { return {}; }

    private:

        static constexpr value_type invalid_value = std::numeric_limits<value_type>::max();

        value_type value_{invalid_value};

    };

}}//qor::par

template <>
struct std::hash<qor::par::workitem_id>
{
    std::size_t operator()(qor::par::workitem_id id) const noexcept
    {
        return std::hash<qor::par::workitem_id::value_type>{}(static_cast<qor::par::workitem_id::value_type>(id));
    }
};


#endif//QOR_PP_H_PARALLEL_WORKITEM_ID
