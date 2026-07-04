// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_SHAPING_MACHINERY
#define QOR_PP_H_TEXT_SHAPING_MACHINERY

namespace qor{ namespace qb{

    //Casts
 
    // StructAtOffset<T>(P,Ofs) returns the struct T& at memory location pointed to by P plus offset bytes.
    template<typename Type>
    static inline const Type& StructAtOffset(const void *P, unsigned int offset)
    { return * reinterpret_cast<const Type*> ((const char *) P + offset); }

    template<typename Type>
    static inline Type& StructAtOffset(void *P, unsigned int offset)
    { return * reinterpret_cast<Type*> ((char *) P + offset); }

    template<typename Type>
    static inline const Type& StructAtOffsetUnaligned(const void *P, unsigned int offset)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
        return * reinterpret_cast<const Type*> ((const char *) P + offset);
#pragma GCC diagnostic pop
    }

    template<typename Type>
    static inline Type& StructAtOffsetUnaligned(void *P, unsigned int offset)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
        return * reinterpret_cast<Type*> ((char *) P + offset);
#pragma GCC diagnostic pop
    }

}}

#endif//QOR_PP_H_TEXT_SHAPING_MACHINERY

