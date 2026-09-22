// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCES_TTF_READER_STATES_TTFINITIAL
#define QOR_PP_H_FRAMEWORK_RESOURCES_TTF_READER_STATES_TTFINITIAL

#include <type_traits>
#include <vector>
#include "../ttfstructures.h"
#include "src/qor/reflection/reflection.h"
#include "src/components/io/deserializer/structstate.h"

namespace qor{ namespace framework{ namespace res {

    class TTFOffsetTableState : public qor::components::serial::StructureDeserializer<offset_subtable>::base
    {
    public:
        TTFOffsetTableState(qor::components::serial::Deserializer* deserializer, offset_subtable* ptd);
        virtual ~TTFOffsetTableState()
        {
        }

    protected:
        offset_subtable* m_initialOffsetTable{ nullptr };
    };

    class TTFTableDirState : public qor::components::serial::StructureDeserializer<table_directory>::base
    {
    public:
        TTFTableDirState(qor::components::serial::Deserializer* deserializer, table_directory* ptd);
        virtual ~TTFTableDirState()
        {
        }

    protected:
        table_directory* m_tableDirectory{ nullptr };
    };

    class TTFBlobState : public workflow::State
    {
    public:

        TTFBlobState(qor::components::serial::Deserializer* deserializer, std::vector<byte>* pv);
        virtual ~TTFBlobState(){}

    protected:

        byte* m_data{ nullptr };
        std::vector<byte>* m_blob{ nullptr };

        class qor::components::serial::Context* GetContext()
        {
            return dynamic_cast<qor::components::serial::Deserializer*>(m_Workflow)->GetContext();
        }

        class Workflow* Workflow()
        {
            return dynamic_cast<class Workflow*>(m_Workflow);
        }

        class qor::components::serial::Deserializer* GetDeserializer()
        {
            return dynamic_cast<qor::components::serial::Deserializer*>(m_Workflow);
        }

    };

    class TTFInitialState : public workflow::State
    {
    public:

        TTFInitialState(qor::components::serial::Deserializer* deserializer);
        virtual ~TTFInitialState()
        {
            delete[] m_tableDirectory;
        }

    protected:

        offset_subtable m_initialOffsetTable{ 0 };
        table_directory* m_tableDirectory{ nullptr };
        unsigned int m_tableIndex{ 0 };
        std::vector<byte> m_blob;

        ref_of<TTFOffsetTableState>::type m_offsetTableState;

        qor::components::serial::Context* GetContext()
        {
            return dynamic_cast<qor::components::serial::Deserializer*>(m_Workflow)->GetContext();
        }

        class Workflow* Workflow()
        {
            return dynamic_cast<class Workflow*>(m_Workflow);
        }

        qor::components::serial::Deserializer* GetDeserializer()
        {
            return dynamic_cast<qor::components::serial::Deserializer*>(m_Workflow);
        }

    };

}}}//qor::framework::res

#endif//QOR_PP_H_FRAMEWORK_RESOURCES_TTF_READER_STATES_TTFINITIAL