#pragma once

// Copyright 2026 Ilya Korolev
// Licensed under the Apache License, Version 2.0
// SPDX-License-Identifier: Apache-2.0

#include "src/framework/data/contract/contract.h"

#include <cstdint>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

namespace contract_tests {

    struct BaseCounters 
    {
        volatile unsigned long long count = 10;
        volatile unsigned long long error = 2;
        volatile unsigned long long time = 30;
        volatile unsigned long long clock = 40;

        qor_pp_contract(BaseCounters, (count, 1), (error, 2), (time, 3), (clock, 4))
    };

    struct CountingAdapter 
    {
        int fields = 0;
        int id_sum = 0;

        template<class Field, class Value>
        void field(const Field&, const Value&) {
            ++fields;
            id_sum += Field::id;
        }
    };

    struct DebugStringAdapter 
    {
        explicit DebugStringAdapter(std::ostream& out)
            : out(out) {}

        template<class Field, class Object>
        void field(const Field& field, const Object& obj) {
            if (!first) {
                out << ", ";
            }

            first = false;
            out << field.name << '=' << field.get(obj);
        }

        std::ostream& out;
        bool first = true;
    };

    template<class Object>
    std::string debug_string(const Object& obj) 
    {
        std::ostringstream out;
        DebugStringAdapter adapter(out);
        contract::visit(obj, adapter);
        return out.str();
    }

    struct RequestBase 
    {
        int timestamp = 7;
        int service = 11;

        qor_pp_contract(RequestBase, (timestamp, 1), (service, 2))
    };

    struct RequestEvent : public RequestBase 
    {
        int user_id = 42;

        qor_pp_contract(RequestEvent, BASE(RequestBase, 1000), (user_id, 1101))
    };

    struct TraceContext 
    {
        int trace_id = 5;

        qor_pp_contract(TraceContext, (trace_id, 1))
    };

    struct RoutedEvent : public RequestBase, public TraceContext 
    {
        int route_id = 9;

        qor_pp_contract(RoutedEvent,
            BASE(RequestBase, 1000),
            BASE(TraceContext, 2000),
            (route_id, 3001))
    };

    struct HookedMetric 
    {
        int raw_count = 0;

        qor_pp_contract(HookedMetric, (raw_count, 1))

        int contract_get(const contract_fields::raw_count&) const 
        {
            return raw_count * 10;
        }

        template<class Value>
        void contract_set(const contract_fields::raw_count&, Value&& value) 
        {
            raw_count = static_cast<int>(std::forward<Value>(value)) / 10;
        }
    };

    struct PlainMetric 
    {
        int raw_count = 0;

        qor_pp_contract(PlainMetric, (raw_count, 1))
    };

    struct RequestStat 
    {
        std::uint64_t started_ns = 100;
        std::uint64_t finished_ns = 150;

        qor_pp_contract(RequestStat,
            PROPERTY(duration_ns, 10, std::uint64_t),
            (started_ns, 11),
            (finished_ns, 12))

        std::uint64_t contract_get(const contract_fields::duration_ns&) const {
            return finished_ns - started_ns;
        }

        void contract_set(const contract_fields::duration_ns&, std::uint64_t value) {
            finished_ns = started_ns + value;
        }
    };

    struct ReferenceFallbackMetric 
    {
        std::uint64_t storage = 0;
        std::uint64_t& raw_count;

        ReferenceFallbackMetric()
            : raw_count(storage) {}

        qor_pp_contract(ReferenceFallbackMetric, REFERENCE(raw_count, 1))
    };

    struct ReferenceHookMetric 
    {
        std::uint64_t storage = 0;
        std::uint64_t& raw_count;

        ReferenceHookMetric()
            : raw_count(storage) {}

        qor_pp_contract(ReferenceHookMetric, REFERENCE(raw_count, 1))

        std::uint64_t contract_get(const contract_fields::raw_count&) const {
            return raw_count * 10;
        }

        template<class Value>
        void contract_set(const contract_fields::raw_count&, Value&& value) {
            raw_count = static_cast<std::uint64_t>(std::forward<Value>(value)) / 10;
        }
    };

    struct ReferenceFreeMetric 
    {
        std::uint64_t storage = 0;
        std::uint64_t& raw_count;

        ReferenceFreeMetric()
            : raw_count(storage) {}

        qor_pp_contract(ReferenceFreeMetric, REFERENCE(raw_count, 1))
    };

    template<class Field>
    std::uint64_t contract_get(const Field&, const ReferenceFreeMetric& metric) 
    {
        static_assert(Field::id == 1);
        return metric.raw_count * 100;
    }

    template<class Field, class Value>
    void contract_set(const Field&, ReferenceFreeMetric& metric, Value&& value) 
    {
        static_assert(Field::id == 1);
        metric.raw_count = static_cast<std::uint64_t>(std::forward<Value>(value)) / 100;
    }

    struct FreeHookMetric 
    {
        int raw_count = 0;

        qor_pp_contract(FreeHookMetric, (raw_count, 1))
    };

    template<class Field>
    int contract_get(const Field&, const FreeHookMetric& metric) 
    {
        static_assert(Field::id == 1);
        return metric.raw_count * 100;
    }

    template<class Field, class Value>
    void contract_set(const Field&, FreeHookMetric& metric, Value&& value) 
    {
        static_assert(Field::id == 1);
        metric.raw_count = static_cast<int>(std::forward<Value>(value)) / 100;
    }

    struct FreeTagMetric 
    {
        int raw_count = 0;

        qor_pp_contract(FreeTagMetric, (raw_count, 1))
    };

    inline int contract_get(const FreeTagMetric::contract_fields::raw_count&, const FreeTagMetric& metric) 
    {
        return metric.raw_count * 1000;
    }

    template<class Value>
    inline void contract_set(const FreeTagMetric::contract_fields::raw_count&, FreeTagMetric& metric, Value&& value) 
    {
        metric.raw_count = static_cast<int>(std::forward<Value>(value)) / 1000;
    }

    struct FreePropertyStat 
    {
        std::uint64_t low = 20;
        std::uint64_t high = 45;

        qor_pp_contract(FreePropertyStat,
            PROPERTY(delta, 5, std::uint64_t),
            (low, 6),
            (high, 7))
    };

    template<class Field, std::enable_if_t<std::is_same_v<Field, FreePropertyStat::contract_fields::delta>, int> = 0>
    std::uint64_t contract_get(const Field&, const FreePropertyStat& stat) 
    {
        return stat.high - stat.low;
    }

    template<class Field, class Value, std::enable_if_t<std::is_same_v<Field, FreePropertyStat::contract_fields::delta>, int> = 0>
    void contract_set(const Field&, FreePropertyStat& stat, Value&& value) 
    {
        stat.high = stat.low + static_cast<std::uint64_t>(std::forward<Value>(value));
    }

    struct FreeTagPropertyStat 
    {
        std::uint64_t low = 30;
        std::uint64_t high = 60;

        qor_pp_contract(FreeTagPropertyStat,
            PROPERTY(delta, 5, std::uint64_t),
            (low, 6),
            (high, 7))
    };

    inline std::uint64_t contract_get(
        const FreeTagPropertyStat::contract_fields::delta&,
        const FreeTagPropertyStat& stat) {
        return stat.high - stat.low;
    }

    template<class Value>
    inline void contract_set(
        const FreeTagPropertyStat::contract_fields::delta&,
        FreeTagPropertyStat& stat,
        Value&& value) 
    {
        stat.high = stat.low + static_cast<std::uint64_t>(std::forward<Value>(value));
    }

    struct ReferenceMetric 
    {
        std::uint64_t storage = 0;
        std::uint64_t& raw_count;

        ReferenceMetric()
            : raw_count(storage) {}

        qor_pp_contract(ReferenceMetric, REFERENCE(raw_count, 1))
    };

    inline std::uint64_t contract_get(const ReferenceMetric::contract_fields::raw_count&, const ReferenceMetric& metric) 
    {
        return metric.raw_count * 10;
    }

    template<class Value>
    inline void contract_set(const ReferenceMetric::contract_fields::raw_count&, ReferenceMetric& metric, Value&& value) 
    {
        metric.raw_count = static_cast<std::uint64_t>(std::forward<Value>(value)) / 10;
    }

}//contract_tests
