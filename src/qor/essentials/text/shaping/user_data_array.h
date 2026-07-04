// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_SHAPING_USER_DATA_ARRAY
#define QOR_PP_H_TEXT_SHAPING_USER_DATA_ARRAY

#include "common.h"
#include "user_data_key.h"
#include "mutex.h"
#include "lockable_set.h"

namespace qor{ namespace qb{

    struct user_data_array_t
    {
        struct user_data_item_t 
        {
            user_data_key_t *key;
            void *data;
            destroy_func_t destroy;

            bool operator == (const user_data_key_t *other_key) const { return key == other_key; }
            bool operator == (const user_data_item_t &other) const { return key == other.key; }

            void fini () { if (destroy) destroy (data); }
        };

        mutex_t lock;
        lockable_set_t<user_data_item_t, mutex_t> items;

        void init () { /*lock.init ();*/ items.init (); }

        void fini () { items.fini (lock); /*lock.fini ();*/ }

        bool set (user_data_key_t *key, void* data, destroy_func_t destroy, bool_t replace)
        {
            if (!key)
            return false;

            if (replace) 
            {
                if (!data && !destroy) 
                {
                    items.remove (key, lock);
                    return true;
                }
            }
            user_data_item_t item = {key, data, destroy};
            bool ret = !!items.replace_or_insert (item, lock, (bool) replace);

            return ret;
        }

        void *get (user_data_key_t *key)
        {
            user_data_item_t item = {nullptr, nullptr, nullptr};
            return items.find (key, &item, lock) ? item.data : nullptr;
        }
    };

}}

#endif//QOR_PP_H_TEXT_SHAPING_USER_DATA_ARRAY