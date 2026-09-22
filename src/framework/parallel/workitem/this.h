// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PARALLEL_WORKITEM_THIS
#define QOR_PP_H_PARALLEL_WORKITEM_THIS

#include "id.h"

#include "detail/noncopyable.h"
#include "detail/nonmovable.h"

namespace qor{ namespace par{
    
    struct alignas(64) this_contract;

    qor_pp_export this_contract*& GetCurrentWorkItem();

    // gives the work function access to its own contract while it is executing.
    // the owning group is reached through an opaque pointer plus thunks so that
    // this type need not be templated on the group's subtree size.
    struct alignas(64) this_contract : detail::non_copyable, detail::non_movable
    {
        this_contract
        (
            workitem_id id,
            void * group,
            void(* release)(workitem_id, void *),
            void(* schedule)(workitem_id, void *)
        ) noexcept :
            prev_(GetCurrentWorkItem()),
            id_(id),
            group_(group),
            release_(release),
            schedule_(schedule)
        {
            GetCurrentWorkItem() = this;
        }

        ~this_contract() noexcept
        {
            GetCurrentWorkItem() = prev_;
        }

        static void schedule() noexcept {GetCurrentWorkItem()->schedule_(GetCurrentWorkItem()->id_, GetCurrentWorkItem()->group_);}

        static void release() noexcept {GetCurrentWorkItem()->release_(GetCurrentWorkItem()->id_, GetCurrentWorkItem()->group_);}

        static auto get_id() noexcept {return GetCurrentWorkItem()->id_;}

        static bool is_executing() noexcept {return (GetCurrentWorkItem() != nullptr);}

        static thread_local this_contract * tlsThisContract_;

        this_contract *     prev_;
        workitem_id    id_;
        void *              group_;

        void(* release_)(workitem_id, void *);
        void(* schedule_)(workitem_id, void *);

    };

}}//qor::par

#endif//QOR_PP_H_PARALLEL_WORKITEM_THIS