// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCES_TTF_READER_STATES_DESIGNATOR
#define QOR_PP_H_FRAMEWORK_RESOURCES_TTF_READER_STATES_DESIGNATOR

#include "src/components/io/deserializer/state.h"

namespace qor{ namespace framework{ namespace res {
    
    class DesignatorState : public qor::components::serial::DeserializerState
    {
    public:

        DesignatorState(qor::components::serial::Deserializer* deserializer);
        virtual ~DesignatorState() = default;    
        
        uint32_t GetObject()
        {
            return m_designator;
        }
        
    protected:
        uint32_t m_designator;
    };

}}}//qor::framework::res

#endif//QOR_PP_H_FRAMEWORK_RESOURCES_TTF_READER_STATES_DESIGNATOR