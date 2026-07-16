
#include <string.h>
#include <type_traits>

namespace qor{ namespace qb{

    static inline void* hb_memset (void* s, int c, unsigned int n)
    {
        if (!n) return s; // It's illegal to pass NULL to memset(), even if n is zero.
        return memset (s, c, n);
    }

    template <typename T> static inline bool
    hb_in_range (T u, T lo, T hi)
    {
        static_assert (!std::is_signed<T>::value, "");

        /* The casts below are important as if T is smaller than int,
        * the subtract results will become a signed int! */
        return (T)(u - lo) <= (T)(hi - lo);
    }

    template <typename T> static inline bool
    hb_in_ranges (T u, T lo1, T hi1)
    {
        return hb_in_range (u, lo1, hi1);
    }

    template <typename T, typename ...Ts> static inline bool
    hb_in_ranges (T u, T lo1, T hi1, Ts... ds)
    {
        return hb_in_range<T> (u, lo1, hi1) || hb_in_ranges<T> (u, ds...);
    }

    template <typename Type, unsigned int n>
    static inline unsigned int ARRAY_LENGTH (const Type (&)[n]) { return n; }


    template <typename V, typename K, typename ...Ts>
    static inline bool hb_bsearch_impl (unsigned *pos, /* Out */ const K& key, V* base, size_t nmemb, size_t stride, int (*compar)(const void *_key, const void *_item, Ts... _ds), Ts... ds)
    {
        int min = 0, max = (int) nmemb - 1;
        while (min <= max)
        {
            int mid = ((unsigned int) min + (unsigned int) max) / 2;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
            V* p = (V*) (((const char *) base) + (mid * stride));
#pragma GCC diagnostic pop
            int c = compar ((const void *) std::addressof (key), (const void *) p, ds...);
            if (c < 0)
                max = mid - 1;
            else if (c > 0)
                min = mid + 1;
            else
            {
                *pos = mid;
                return true;
            }
        }
        *pos = min;
        return false;
    }

    template <typename V, typename K>
    static inline V* hb_bsearch (const K& key, V* base, size_t nmemb, size_t stride /*= sizeof (V)*/, int (*compar)(const void *_key, const void *_item) /*= _hb_cmp_method<K, V>*/)
    {
        unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
        return hb_bsearch_impl (&pos, key, base, nmemb, stride, compar) ?
            (V*) (((const char *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic pop
    }

    template <typename V, typename K, typename ...Ts>
    static inline V* hb_bsearch (const K& key, V* base, size_t nmemb, size_t stride, int (*compar)(const void *_key, const void *_item, Ts... _ds), Ts... ds)
    {
        unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
        return hb_bsearch_impl (&pos, key, base, nmemb, stride, compar, ds...) ?
            (V*) (((const char *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic pop
    }


}}//qor::qb
