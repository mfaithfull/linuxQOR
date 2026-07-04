// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_SHAPING_ARRAY
#define QOR_PP_H_TEXT_SHAPING_ARRAY

namespace qor{ namespace qb{

    template <typename Type>
    struct sorted_array_t;

    enum hb_not_found_t
    {
    HB_NOT_FOUND_DONT_STORE,
    HB_NOT_FOUND_STORE,
    HB_NOT_FOUND_STORE_CLOSEST,
    };


template <typename Type>
struct array_t : hb_iter_with_fallback_t<array_t<Type>, Type&>
{
  static constexpr bool realloc_move = true;

  //Constructors.
   
  array_t () = default;
  array_t (const array_t&) = default;
  ~array_t () = default;
  array_t& operator= (const array_t&) = default;
  array_t& operator= (array_t&&) = default;

  constexpr array_t (Type *array_, unsigned int length_) : arrayZ (array_), length (length_) {}
  template <unsigned int length_>
  constexpr array_t (Type (&array_)[length_]) : array_t (array_, length_) {}

  template <typename U, hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr array_t (const array_t<U> &o) : hb_iter_with_fallback_t<array_t, Type&> (), arrayZ (o.arrayZ), length (o.length), backwards_length (o.backwards_length) {}

  template <typename U, hb_enable_if (hb_is_cr_convertible(U, Type))>
  array_t& operator = (const array_t<U> &o)
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; return *this; }

  //Iterator implementation.
   
  typedef Type& __item_t__;
  static constexpr bool is_random_access_iterator = true;
  static constexpr bool has_fast_len = true;
  Type& __item__ () const
  {
    if (unlikely (!length)) return CrapOrNull (Type);
    return *arrayZ;
  }
  Type& __item_at__ (unsigned i) const
  {
    if (unlikely (i >= length)) return CrapOrNull (Type);
    return arrayZ[i];
  }
  void __next__ ()
  {
    if (unlikely (!length))
      return;
    length--;
    backwards_length++;
    arrayZ++;
  }
  void __forward__ (unsigned n)
  {
    if (unlikely (n > length))
      n = length;
    length -= n;
    backwards_length += n;
    arrayZ += n;
  }
  void __prev__ ()
  {
    if (unlikely (!backwards_length))
      return;
    length++;
    backwards_length--;
    arrayZ--;
  }
  void __rewind__ (unsigned n)
  {
    if (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  unsigned __len__ () const { return length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using array_t for range-based loop before. */
  bool operator != (const array_t& o) const
  { return this->arrayZ != o.arrayZ || this->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () const { return arrayZ; }
  Type *end () const { return arrayZ + length; }


  /* Extra operators.
   */
  Type * operator & () const { return arrayZ; }
  operator array_t<const Type> () { return array_t<const Type> (arrayZ, length); }
  template <typename T> operator T * () const { return arrayZ; }

  HB_INTERNAL bool operator == (const array_t &o) const;

  uint32_t hash () const
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    for (auto &v : *this)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    return current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  int cmp (const array_t &a) const
  {
    if (length != a.length)
      return (int) a.length - (int) length;
    return hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL static int cmp (const void *pa, const void *pb)
  {
    array_t *a = (array_t *) pa;
    array_t *b = (array_t *) pb;
    return b->cmp (*a);
  }

  template <typename T>
  Type *lsearch (const T &x, Type *not_found = nullptr)
  {
    unsigned i;
    return lfind (x, &i) ? &this->arrayZ[i] : not_found;
  }
  template <typename T>
  const Type *lsearch (const T &x, const Type *not_found = nullptr) const
  {
    unsigned i;
    return lfind (x, &i) ? &this->arrayZ[i] : not_found;
  }
  template <typename T>
  bool lfind (const T &x, unsigned *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned int to_store = (unsigned int) -1) const
  {
    for (unsigned i = 0; i < length; ++i)
      if (hb_equal (x, this->arrayZ[i]))
      {
	if (pos)
	  *pos = i;
	return true;
      }

    if (pos)
    {
      switch (not_found)
      {
	case HB_NOT_FOUND_DONT_STORE:
	  break;

	case HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  break;

	case HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = length;
	  break;
      }
    }
    return false;
  }

  sorted_array_t<Type> qsort (int (*cmp)(const void*, const void*))
  {
    if (likely (length))
      hb_qsort (arrayZ, length, this->get_item_size (), cmp);
    return sorted_array_t<Type> (*this);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  template <typename Compar>
  sorted_array_t<Type> qsort (Compar compar)
  {
    if (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    return sorted_array_t<Type> (*this);
  }

  private:
  template <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    return qsort ([] (const Type &a, const Type &b) { return Type::cmp (&a, &b); });
  }
  sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    return qsort ((int(*)(const void*, const void*)) Type::cmp);
  }
  public:

  sorted_array_t<Type> qsort ()
  {
    return _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_size () const { return length * this->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  void reverse (unsigned start = 0, unsigned end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    if (end < start + 2)
      return;

    unsigned stop = start + (end - start) / 2;
    for (unsigned lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  array_t sub_array (unsigned int start_offset = 0, unsigned int *seg_count = nullptr /* IN/OUT */) const
  {
    if (!start_offset && !seg_count)
      return *this;

    unsigned int count = length;
    if (unlikely (start_offset > count))
      count = 0;
    else
      count -= start_offset;
    if (seg_count)
      count = *seg_count = hb_min (count, *seg_count);
    return array_t (arrayZ + start_offset, count);
  }
  array_t sub_array (unsigned int start_offset, unsigned int seg_count) const
  { return sub_array (start_offset, &seg_count); }

  array_t truncate (unsigned length) const { return sub_array (0, length); }

  template <typename T,
	    unsigned P = sizeof (Type),
	    hb_enable_if (P == 1)>
  const T *as () const
  { return length < hb_min_size (T) ? &Null (T) : reinterpret_cast<const T *> (arrayZ); }

  template <typename T,
	    unsigned P = sizeof (Type),
	    hb_enable_if (P == 1)>
  bool check_range (const T *p, unsigned int size = T::static_size) const
  {
    return arrayZ <= ((const char *) p)
	&& ((const char *) p) <= arrayZ + length
	&& (unsigned int) (arrayZ + length - (const char *) p) >= size;
  }

  template <unsigned P = sizeof (Type),
	    hb_enable_if (P == 1)>
  bool check_end (const void *p) const
  {
    return (uintptr_t) (((const char *) p) - arrayZ) <= length;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  void fini ()
  { hb_free ((void *) arrayZ); arrayZ = nullptr; length = 0; }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(sizeof (U) < sizeof (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  array_t copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    auto* out = c->start_embed (arrayZ);
    if (unlikely (!c->extend_size (out, get_size (), false))) return_trace (array_t ());
    for (unsigned i = 0; i < length; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (array_t (out, length));
  }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (sizeof (U) < sizeof (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  array_t copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    auto* out = c->start_embed (arrayZ);
    if (unlikely (!c->extend_size (out, get_size (), false))) return_trace (array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (array_t (out, length));
  }

  template <typename hb_sanitize_context_t>
  bool sanitize (hb_sanitize_context_t *c) const
  { return c->check_array (arrayZ, length); }

  /*
   * Members
   */

  public:
  Type *arrayZ = nullptr;
  unsigned int length = 0;
  unsigned int backwards_length = 0;
};
template <typename T> inline array_t<T>
hb_array ()
{ return array_t<T> (); }
template <typename T> inline array_t<T>
hb_array (T *array, unsigned int length)
{ return array_t<T> (array, length); }
template <typename T, unsigned int length_> inline array_t<T>
hb_array (T (&array_)[length_])
{ return array_t<T> (array_); }

template <typename Type>
struct sorted_array_t :
	array_t<Type>,
	hb_iter_t<sorted_array_t<Type>, Type&>
{
  typedef hb_iter_t<sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  static constexpr bool is_random_access_iterator = true;
  static constexpr bool is_sorted_iterator = true;
  static constexpr bool has_fast_len = true;

  sorted_array_t () = default;
  sorted_array_t (const sorted_array_t&) = default;
  ~sorted_array_t () = default;
  sorted_array_t& operator= (const sorted_array_t&) = default;
  sorted_array_t& operator= (sorted_array_t&&) = default;

  constexpr sorted_array_t (Type *array_, unsigned int length_) : array_t<Type> (array_, length_) {}
  template <unsigned int length_>
  constexpr sorted_array_t (Type (&array_)[length_]) : array_t<Type> (array_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr sorted_array_t (const array_t<U> &o) :
    array_t<Type> (o),
    hb_iter_t<sorted_array_t, Type&> () {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  sorted_array_t& operator = (const array_t<U> &o)
  { array_t<Type> (*this) = o; return *this; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  bool operator != (const sorted_array_t& o) const
  { return this->arrayZ != o.arrayZ || this->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () const { return this->arrayZ; }
  Type *end () const { return this->arrayZ + this->length; }


  sorted_array_t sub_array (unsigned int start_offset, unsigned int *seg_count /* IN/OUT */) const
  { return sorted_array_t (((const array_t<Type> *) (this))->sub_array (start_offset, seg_count)); }
  sorted_array_t sub_array (unsigned int start_offset, unsigned int seg_count) const
  { return sub_array (start_offset, &seg_count); }

  sorted_array_t truncate (unsigned length) const { return sub_array (0, length); }

  template <typename T>
  Type *bsearch (const T &x, Type *not_found = nullptr)
  {
    unsigned int i;
    return bfind (x, &i) ? &this->arrayZ[i] : not_found;
  }
  template <typename T>
  const Type *bsearch (const T &x, const Type *not_found = nullptr) const
  {
    unsigned int i;
    return bfind (x, &i) ? &this->arrayZ[i] : not_found;
  }
  template <typename T>
  bool bfind (const T &x, unsigned int *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned int to_store = (unsigned int) -1) const
  {
    unsigned pos;

    if (bsearch_impl (x, &pos))
    {
      if (i)
	*i = pos;
      return true;
    }

    if (i)
    {
      switch (not_found)
      {
	case HB_NOT_FOUND_DONT_STORE:
	  break;

	case HB_NOT_FOUND_STORE:
	  *i = to_store;
	  break;

	case HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  break;
      }
    }
    return false;
  }
  template <typename T, typename ...Ts>
  bool bsearch_impl (const T &x, unsigned *pos, Ts... ds) const
  {
    return hb_bsearch_impl (pos,
			    x,
			    this->arrayZ,
			    this->length,
			    sizeof (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
template <typename T> inline sorted_array_t<T>
hb_sorted_array (T *array, unsigned int length)
{ return sorted_array_t<T> (array, length); }
template <typename T, unsigned int length_> inline sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ return sorted_array_t<T> (array_); }

template <typename T>
inline bool array_t<T>::operator == (const array_t<T> &o) const
{
  if (o.length != this->length) return false;
  for (unsigned int i = 0; i < this->length; i++) {
    if (this->arrayZ[i] != o.arrayZ[i]) return false;
  }
  return true;
}
template <>
inline bool array_t<const char>::operator == (const array_t<const char> &o) const
{
  if (o.length != this->length) return false;
  return 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
template <>
inline bool array_t<const unsigned char>::operator == (const array_t<const unsigned char> &o) const
{
  if (o.length != this->length) return false;
  return 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
template <>
inline uint32_t array_t<const char>::hash () const
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  return fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

template <>
inline uint32_t array_t<const unsigned char>::hash () const
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  return fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


typedef array_t<const char> hb_bytes_t;
typedef array_t<const unsigned char> hb_ubytes_t;



}}

#endif//QOR_PP_H_TEXT_SHAPING_ARRAY