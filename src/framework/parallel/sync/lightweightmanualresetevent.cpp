// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"


#include "lightweightmanualresetevent.h"

//prefer a platform specific variant over this not so lightweight platfrom generic version

namespace qor{ namespace detail{

    #include <system_error>

    LightManualResetEvent::LightManualResetEvent() noexcept : m_isSet(false){ }

    LightManualResetEvent::LightManualResetEvent(bool initiallySet) noexcept : m_isSet(initiallySet){ }
    
    LightManualResetEvent::~LightManualResetEvent(){ }
    
    void LightManualResetEvent::Set() noexcept
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_isSet = true;
        m_cv.notify_all();
    }
    
    void LightManualResetEvent::Reset() noexcept
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_isSet = false;
    }
    
    void LightManualResetEvent::Wait() noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this] { return m_isSet; });
    }
    
}}//qor::detail
