// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_LOG_LEVELWHAT
#define QOR_PP_H_LOG_LEVELWHAT

#include "src/qor/flyers/issue/what.h"
#include "level.h"
#include "src/qor/memory/reference/reference.h"

namespace qor{ namespace log {

    class qor_pp_module_interface(QOR_LOG) LevelWhat : public What
    {
    public:

        qor_pp_noinline LevelWhat(const ref_of<LevelWhat>::type& src);
        qor_pp_noinline LevelWhat(const LevelWhat& src);
        qor_pp_noinline LevelWhat(const std::string& what, Level level);
        qor_pp_noinline virtual ~LevelWhat() noexcept;
        qor_pp_noinline LevelWhat& operator = (const LevelWhat& src);
        qor_pp_noinline Level GetLevel() const;
        
    protected:

        Level m_level;
    };

}}//qor::log

#endif//QOR_PP_H_LOG_LEVELWHAT
