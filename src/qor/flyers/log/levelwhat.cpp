// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "levelwhat.h"

namespace qor{ namespace log {

    LevelWhat::LevelWhat(const ref_of<LevelWhat>::type& src) : What(src->Content()), m_level(src->m_level){ }

    LevelWhat::LevelWhat(const LevelWhat& src) : What(src.Content()), m_level(src.m_level){ }

    LevelWhat::LevelWhat(const std::string& what, Level level) : What(what), m_level(level){ }

    LevelWhat::~LevelWhat() noexcept = default;

    LevelWhat& LevelWhat::operator = (const LevelWhat& src)
    {
        if(&src != this)
        {
            m_level = src.m_level;
        }
        return *this;
    }

    Level LevelWhat::GetLevel() const
    {
        return m_level;
    }
        
}}//qor::log
