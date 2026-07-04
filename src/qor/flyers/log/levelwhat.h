// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_LOG_LEVELWHAT
#define QOR_PP_H_LOG_LEVELWHAT

#include "src/qor/flyers/issue/what.h"
#include "level.h"

namespace qor{ namespace log {

    class qor_pp_module_interface(QOR_LOG) LevelWhat : public What
    {
    public:

        LevelWhat(const ref_of<LevelWhat>::type& src) : What(src->Content()), m_level(src->m_level){ }

        LevelWhat(const std::string& what, Level level) : What(what), m_level(level){ }

        virtual ~LevelWhat() noexcept = default;

        Level GetLevel() const
        {
            return m_level;
        }
        
    protected:

        Level m_level;
    };

}}//qor::log

#endif//QOR_PP_H_LOG_LEVELWHAT
