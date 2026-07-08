// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "designator.h"

namespace qor { namespace framework { namespace res {

    DesignatorState::DesignatorState(qor::components::serial::Deserializer* deserializer) : qor::components::serial::DeserializerState(deserializer, sizeof(uint32_t), arch::Endian::big)    
    {
        m_designator = 0;
        m_data = reinterpret_cast<byte*>(&m_designator);        
    }
    
}}}//qor::framework::res
