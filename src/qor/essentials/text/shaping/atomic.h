// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_SHAPING_ATOMIC
#define QOR_PP_H_TEXT_SHAPING_ATOMIC

#include <atomic>

namespace qor{ namespace qb{

    template <typename T>
    struct atomic_t
    {
        atomic_t () = default;
        constexpr atomic_t (T v) : v (v) {}
        constexpr atomic_t (const atomic_t& o) : v (o.get_relaxed ()) {}
        constexpr atomic_t (atomic_t&& o) : v (o.get_relaxed ()) { o.set_relaxed ({}); }

        atomic_t &operator= (const atomic_t& o) { set_relaxed (o.get_relaxed ()); return *this; }
        atomic_t &operator= (atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); return *this; }

        atomic_t &operator= (T v_)
        {
            set_relaxed (v_);
            return *this;
        }

        operator T () const { return get_relaxed (); }

        void set_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
        void set_release (T v_) { v.store (v_, std::memory_order_release); }
        T get_relaxed () const { return v.load (std::memory_order_relaxed); }
        T get_acquire () const { return v.load (std::memory_order_acquire); }
        T inc () { return v.fetch_add (1, std::memory_order_acq_rel); }
        T dec () { return v.fetch_add (-1, std::memory_order_acq_rel); }

        int operator++ (int) { return inc (); }
        int operator-- (int) { return dec (); }

        friend void swap (atomic_t &a, atomic_t &b) noexcept
        {
            T v = a.get_acquire ();
            a.set_relaxed (b.get_acquire ());
            b.set_relaxed (v);
        }

        std::atomic<T> v = 0;
    };

    template <typename T>
    struct atomic_t<T *>
    {
        atomic_t () = default;
        constexpr atomic_t (T *v) : v (v) {}
        atomic_t (const atomic_t &other) = delete;

        void init (T *v_ = nullptr) { set_relaxed (v_); }
        void set_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
        T *get_relaxed () const { return v.load (std::memory_order_relaxed); }
        T *get_acquire () const { return v.load (std::memory_order_acquire); }
        bool cmpexch (T *old, T *new_) { return v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

        atomic_t &operator= (const atomic_t& o) { set_relaxed (o.get_relaxed ()); return *this; }
        atomic_t &operator= (atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); return *this; }

        operator bool () const { return get_acquire () != nullptr; }
        T *operator->() const { return get_acquire (); }
        template <typename C>
        operator C * () const
        {
            return get_acquire ();
        }

        friend void swap (atomic_t &a, atomic_t &b) noexcept
        {
            T *p = a.get_acquire ();
            a.set_relaxed (b.get_acquire ());
            b.set_relaxed (p);
        }

        std::atomic<T *> v = nullptr;
    };

}}

#endif//QOR_PP_H_TEXT_SHAPING_ATOMIC