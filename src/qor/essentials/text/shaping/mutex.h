// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_SHAPING_MUTEX
#define QOR_PP_H_TEXT_SHAPING_MUTEX

#include <mutex>

namespace qor{ namespace qb{

    using mutex_t = std::recursive_mutex;

}}

#endif//QOR_PP_H_TEXT_SHAPING_MUTEX