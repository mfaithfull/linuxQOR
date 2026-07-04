// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_SHAPING_LOCKABLE_SET
#define QOR_PP_H_TEXT_SHAPING_LOCKABLE_SET

#include "vector.hh"

namespace qor{ namespace qb{

    template <typename item_t, typename lock_t>
    struct lockable_set_t
    {
        vector_t<item_t> items;

        void init () { items.init (); }

        template <typename T>
        item_t *replace_or_insert (T v, lock_t &l, bool replace)
        {
            l.lock ();
            item_t *item = items.lsearch (v);
            if (item) 
            {
                if (replace) 
                {
                    item_t old = *item;
                    *item = v;
                    l.unlock ();
                    old.fini ();
                }
                else 
                {
                    item = nullptr;
                    l.unlock ();
                }
            } 
            else 
            {
                item = items.push (v);
                l.unlock ();
            }
            return items.in_error () ? nullptr : item;
        }

        template <typename T>
        void remove (T v, lock_t &l)
        {
            l.lock ();
            item_t *item = items.lsearch (v);
            if (item)
            {
                item_t old = *item;
                *item = std::move (items.tail ());
                items.pop ();
                l.unlock ();
                old.fini ();
            } 
            else 
            {
                l.unlock ();
            }
        }

        template <typename T>
        bool find (T v, item_t *i, lock_t &l)
        {
            l.lock ();
            item_t *item = items.lsearch (v);
            if (item)
            {
                *i = *item;
            }
            l.unlock ();
            return !!item;
        }

        template <typename T>
        item_t *find_or_insert (T v, lock_t &l)
        {
            l.lock ();
            item_t *item = items.find (v);
            if (!item) 
            {
                item = items.push (v);
            }
            l.unlock ();
            return item;
        }

        void fini (lock_t &l)
        {
            if (!items.length)
            {
                // No need to lock.
                items.fini ();
                return;
            }
            l.lock ();
            while (items.length)
            {
                item_t old = items.tail ();
                items.pop ();
                l.unlock ();
                old.fini ();
                l.lock ();
            }
            items.fini ();
            l.unlock ();
        }

    };

}}

#endif//QOR_PP_H_TEXT_SHAPING_LOCKABLE_SET