// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PARALLEL_WORKITEM_ITEM
#define QOR_PP_H_PARALLEL_WORKITEM_ITEM

#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>

#include "id.h"
#include "detail/noncopyable.h"
#include "src/framework/parallel/signaltree/detail/synchronizationmode.h"

namespace qor { namespace par{

    template <size_t, synchronization_mode> class workitem_group;

    class qor_pp_module_interface(QOR_WORKITEM) workitem final : detail::non_copyable
    {
    public:

        enum class initial_state
        {
            unscheduled = 0,
            scheduled   = 1
        };

        workitem() noexcept = default;

        ~workitem();

        workitem(workitem &&) noexcept;
        workitem & operator = (workitem &&) noexcept;

        void schedule() noexcept;

        bool release() noexcept;

        bool is_valid() const noexcept;

        explicit operator bool() const noexcept;

    private:

        template <size_t, synchronization_mode> friend class workitem_group;

        using operation = bool (*)(void *, workitem_id, uint64_t) noexcept;
        using operations = std::array<operation, 4>;

        static auto constexpr schedule_operation = 0ull;
        static auto constexpr release_operation = 1ull;
        static auto constexpr is_valid_operation = 2ull;
        static auto constexpr release_reference_operation = 3ull;

        template <typename SharedState>
        static operations const & get_operations() noexcept
        {
            static constexpr operations value
            {
                [](void * state, auto contractId, auto contractGeneration) noexcept
                    {
                        static_cast<SharedState *>(state)->schedule(contractId, contractGeneration);
                        return true;
                    },
                [](void * state, auto contractId, auto contractGeneration) noexcept
                    { return static_cast<SharedState *>(state)->release(contractId, contractGeneration); },
                [](void * state, auto contractId, auto contractGeneration) noexcept
                    { return static_cast<SharedState *>(state)->is_valid(contractId, contractGeneration); },
                [](void * state, auto, auto) noexcept
                    {
                        static_cast<SharedState *>(state)->release_reference();
                        return true;
                    }
            };
            return value;
        }

        void reset() noexcept;

        template <typename SharedState>
        workitem
        (
            SharedState * sharedState,
            workitem_id id,
            uint64_t generation,
            initial_state initialState
        ) noexcept :
            sharedState_(sharedState),
            operations_(&get_operations<SharedState>()),
            id_(id),
            generation_(generation)
        {
            sharedState->add_reference();
            if (initialState == initial_state::scheduled)
                schedule();
        }

        void*               sharedState_{};
        operations const*   operations_{};
        workitem_id    id_;
        uint64_t            generation_{};

    };

}}//qor::par


inline qor::par::workitem::workitem
(
    workitem && other
) noexcept :
    workitem()
{
    *this = std::move(other);
}


inline auto qor::par::workitem::operator =
(
    workitem && other
) noexcept -> workitem &
{
    if (this != &other)
    {
        reset();
        sharedState_ = std::exchange(other.sharedState_, nullptr);
        operations_ = std::exchange(other.operations_, nullptr);
        id_ = std::exchange(other.id_, {});
        generation_ = std::exchange(other.generation_, 0);
    }
    return *this;
}


inline qor::par::workitem::~workitem()
{
    reset();
}


inline void qor::par::workitem::schedule
(
    // the hot path.  the generation is passed through so that a handle which
    // refers to a slot that has since been recycled is silently ignored.
    //
) noexcept
{
    (*operations_)[schedule_operation](sharedState_, id_, generation_);
}


inline bool qor::par::workitem::release() noexcept
{
    if (sharedState_ == nullptr)
        return false;
    return (*operations_)[release_operation](sharedState_, id_, generation_);
}


inline void qor::par::workitem::reset() noexcept
{
    auto state = std::exchange(sharedState_, nullptr);
    if (state == nullptr)
        return;
    (*operations_)[release_operation](state, id_, generation_);
    (*operations_)[release_reference_operation](state, {}, 0);
}


inline bool qor::par::workitem::is_valid() const noexcept
{
    return
        (sharedState_ != nullptr)
        && ((*operations_)[is_valid_operation](sharedState_, id_, generation_));
}


inline qor::par::workitem::operator bool() const noexcept
{
    return is_valid();
}

#endif//QOR_PP_H_PARALLEL_WORKITEM_ITEM
