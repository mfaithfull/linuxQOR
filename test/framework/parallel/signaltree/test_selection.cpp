

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "src/configuration/configuration.h"
#include "src/framework/parallel/signaltree/set.h"
#include "testsupport.h"
#include "src/framework/parallel/signaltree/densestchildselector.h"

namespace
{

    using ready_set = std::vector<uint64_t>;
    using ready_sets = std::vector<ready_set>;


    constexpr auto value(qor::par::signal_id signal) noexcept
    {
        return static_cast<qor::par::signal_id::value_type>(signal);
    }


    void add_probe(std::vector<uint64_t> & probes, uint64_t probe, uint64_t capacity)
    {
        if (probe < capacity)
            probes.push_back(probe);
    }


    void add_boundary(std::vector<uint64_t> & probes, uint64_t boundary, uint64_t capacity)
    {
        if (boundary != 0)
            add_probe(probes, boundary - 1, capacity);
        add_probe(probes, boundary, capacity);
        add_probe(probes, boundary + 1, capacity);
    }


    void normalize(std::vector<uint64_t> & values)
    {
        std::ranges::sort(values);
        values.erase(std::ranges::unique(values).begin(), values.end());
    }


    template <size_t tree_depth>
    auto tree_probes() -> std::vector<uint64_t>
    {
        constexpr auto capacity = test_support::signal_tree<tree_depth>::capacity;
        constexpr auto leafCapacity = test_support::signal_tree<0>::capacity;
        std::vector<uint64_t> probes;

        add_probe(probes, 0, capacity);
        add_probe(probes, 1, capacity);
        add_probe(probes, 3, capacity);
        add_boundary(probes, capacity / 2, capacity);
        add_probe(probes, capacity - 2, capacity);
        add_probe(probes, capacity - 1, capacity);

        for (auto boundary = leafCapacity; boundary < capacity; boundary += leafCapacity)
            add_boundary(probes, boundary, capacity);

        normalize(probes);
        return probes;
    }


    template <size_t tree_depth>
    auto representative_tree_probes() -> std::vector<uint64_t>
    {
        constexpr auto capacity = test_support::signal_tree<tree_depth>::capacity;
        constexpr auto leafCapacity = test_support::signal_tree<0>::capacity;
        std::vector<uint64_t> probes;

        add_probe(probes, 0, capacity);
        add_probe(probes, 1, capacity);
        add_probe(probes, 3, capacity);
        add_boundary(probes, leafCapacity, capacity);
        add_boundary(probes, capacity / 8, capacity);
        add_boundary(probes, capacity / 4, capacity);
        add_boundary(probes, capacity / 2, capacity);
        if (capacity > leafCapacity)
            add_boundary(probes, capacity - leafCapacity, capacity);
        add_probe(probes, capacity - 2, capacity);
        add_probe(probes, capacity - 1, capacity);

        normalize(probes);
        return probes;
    }


    template <size_t tree_depth>
    auto set_probes(size_t shardCount) -> std::vector<uint64_t>
    {
        constexpr auto treeCapacity = test_support::signal_tree<tree_depth>::capacity;
        auto const capacity = treeCapacity * shardCount;
        auto const localProbes = representative_tree_probes<tree_depth>();
        std::vector<uint64_t> probes;
        auto const selectedShards = std::array<size_t, 3>{0, shardCount / 2, shardCount - 1};

        for (auto shard : selectedShards)
            for (auto localProbe : localProbes)
                add_probe(probes, (shard * treeCapacity) + localProbe, capacity);

        for (auto boundary = treeCapacity; boundary < capacity; boundary += treeCapacity)
            add_boundary(probes, boundary, capacity);

        normalize(probes);
        return probes;
    }


    void add_ready_set(ready_sets & sets,ready_set ready)
    {
        normalize(ready);
        if ((not ready.empty()) && (std::ranges::find(sets, ready) == sets.end()))
            sets.push_back(std::move(ready));
    }


    auto make_ready_sets(std::vector<uint64_t> const & probes) -> ready_sets
    {
        ready_sets sets;

        for (size_t index = 0; index < probes.size(); ++index)
        {
            add_ready_set(sets, {probes[index]});

            if (index + 1 < probes.size())
                add_ready_set(sets, {probes[index], probes[index + 1]});

            if (index + 2 < probes.size())
                add_ready_set(sets, {probes[index], probes[index + 1], probes[index + 2]});

            add_ready_set(sets, {probes[index], probes[probes.size() - index - 1]});
        }

        add_ready_set(sets, {probes.front(), probes[probes.size() / 2], probes.back()});
        return sets;
    }


    auto make_hints(std::vector<uint64_t> const & probes, uint64_t capacity ) -> std::vector<qor::par::signal_id>
    {
        std::vector<qor::par::signal_id> hints;
        hints.reserve(probes.size() + 3);
        hints.push_back(qor::par::signal_id::invalid());

        for (auto probe : probes)
            hints.push_back(qor::par::signal_id{probe});

        hints.push_back(qor::par::signal_id{capacity});
        hints.push_back(qor::par::signal_id{capacity + 17});
        return hints;
    }


    auto context(ready_set const & ready, qor::par::signal_id hint) -> std::string
    {
        auto result = std::string{"ready={"};

        for (size_t index = 0; index < ready.size(); ++index)
        {
            if (index != 0)
                result += ',';
            result += std::to_string(ready[index]);
        }

        result += "}, hint=";
        result += hint.valid() ? std::to_string(value(hint)) : std::string{"invalid"};
        return result;
    }


    template <typename Selector, typename Signals>
    void require_selection_contract(Signals & signals, ready_set ready, qor::par::signal_id initialHint)
    {
        auto const failureContext = context(ready, initialHint);
        auto hint = initialHint;

        while (not ready.empty())
        {
            auto const selected = test_support::select_signal<Selector>(signals, hint);
            test_support::require(selected.valid(), failureContext);

            auto const found = std::ranges::find(ready, value(selected));
            test_support::require(found != ready.end(), failureContext);
            ready.erase(found);
        }

        test_support::require(not test_support::select_signal<Selector>(signals, hint).valid(), failureContext);
        test_support::require(not hint.valid(), failureContext);
    }


    template <size_t tree_depth, typename Selector>
    void tree_selection_contract()
    {
        constexpr auto capacity = test_support::signal_tree<tree_depth>::capacity;
        auto const probes = tree_probes<tree_depth>();
        auto const sets = make_ready_sets(probes);
        auto const hints = make_hints(probes, capacity);

        for (auto const & ready : sets)
            for (auto initialHint : hints)
            {
                test_support::signal_tree<tree_depth> tree;

                for (auto signal : ready)
                    test_support::require(test_support::set_signal(tree, qor::par::signal_id{signal}),
                            "tree setup must raise every signal");

                require_selection_contract<Selector>(tree, ready, initialHint);
                test_support::require(tree.empty(), "selection contract must drain the tree");
            }
    }


    template <size_t tree_depth, typename Selector>
    void set_selection_contract()
    {
        constexpr auto treeCapacity = test_support::signal_tree<tree_depth>::capacity;

        for (auto shardCount : std::array<size_t, 3>{1, 3, 9})
        {
            auto const capacity = treeCapacity * shardCount;
            auto const probes = set_probes<tree_depth>(shardCount);
            auto const sets = make_ready_sets(probes);
            auto const hints = make_hints(probes, capacity);

            for (auto const & ready : sets)
                for (auto initialHint : hints)
                {
                    qor::par::signal_set<tree_depth> signals{capacity};

                    for (auto signal : ready)
                        test_support::require(signals.set(qor::par::signal_id{signal}),
                                "signal set setup must raise every signal");

                    require_selection_contract<Selector>(signals, ready, initialHint);
                }
        }
    }

} // namespace


int test_selection()
{
    return test_support::run_suite([]
    {
        test_support::run("signal_tree<0> fairness selection contract", tree_selection_contract<0, qor::par::fairness_selector>);
        test_support::run("signal_tree<1> fairness selection contract", tree_selection_contract<1, qor::par::fairness_selector>);
        test_support::run("signal_tree<2> fairness selection contract", tree_selection_contract<2, qor::par::fairness_selector>);
        test_support::run("signal_tree<0> densest selection contract", tree_selection_contract<0, qor::par::densest_child_selector>);
        test_support::run("signal_tree<1> densest selection contract", tree_selection_contract<1, qor::par::densest_child_selector>);
        test_support::run("signal_tree<2> densest selection contract", tree_selection_contract<2, qor::par::densest_child_selector>);
        test_support::run("signal_set<0> fairness selection contract", set_selection_contract<0, qor::par::fairness_selector>);
        test_support::run("signal_set<1> fairness selection contract", set_selection_contract<1, qor::par::fairness_selector>);
        test_support::run("signal_set<2> fairness selection contract", set_selection_contract<2, qor::par::fairness_selector>);
        test_support::run("signal_set<0> densest selection contract", set_selection_contract<0, qor::par::densest_child_selector>);
        test_support::run("signal_set<1> densest selection contract", set_selection_contract<1, qor::par::densest_child_selector>);
        test_support::run("signal_set<2> densest selection contract", set_selection_contract<2, qor::par::densest_child_selector>);
    });
}
