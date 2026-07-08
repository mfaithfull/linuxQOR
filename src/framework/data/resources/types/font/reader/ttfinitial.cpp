// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "ttfinitial.h"

using namespace qor::components::serial;

namespace qor { namespace framework { namespace res {

    TTFInitialState::TTFInitialState(Deserializer* deserializer) : workflow::State(deserializer), m_initialOffsetTable{0}, m_tableDirectory(nullptr), m_tableIndex(0)
    {
        Enter = [this]()
        {
            m_offsetTableState = new_ref<TTFOffsetTableState>(GetDeserializer(), &m_initialOffsetTable);
            Workflow()->PushState(m_offsetTableState);
        };

        Resume = [this]()
        {
            if(m_tableDirectory == nullptr)
            {
                m_tableDirectory = new table_directory[m_initialOffsetTable.numTables];
            }
            if(m_tableIndex < m_initialOffsetTable.numTables)
            {
                m_tableDirectory[m_tableIndex] = {0};
                Workflow()->PushState(
                    new_ref<TTFTableDirState>(GetDeserializer(), &m_tableDirectory[m_tableIndex]));
                m_tableIndex++;
            }
            else
            {
                Workflow()->SetState(new_ref<TTFBlobState>(GetDeserializer(), &m_blob));
            }
        };
        
    }

    TTFOffsetTableState::TTFOffsetTableState(Deserializer* deserializer, offset_subtable* ptd) :
        StructureDeserializer<offset_subtable>::base(deserializer, arch::Endian::big, ptd), m_initialOffsetTable(ptd)
    {
    }

    TTFTableDirState::TTFTableDirState(Deserializer* deserializer, table_directory* ptd) : 
        StructureDeserializer<table_directory>::base(deserializer, arch::Endian::big, ptd), m_tableDirectory(ptd)
    {
    }

    TTFBlobState::TTFBlobState(qor::components::serial::Deserializer* deserializer, std::vector<byte>* pv) : workflow::State(deserializer), m_blob(pv)
    {
        Enter = [this]()
        {            
            if(GetContext()->GetOctet(m_data))
            {
                m_blob->emplace_back(*m_data);
                GetContext()->ConsumeOctet();
            }
            else
            {
                Workflow()->PopState();
            }
        };
    }
    
}}}//qor::framework::res

/*
                if(m_tableIndex > 0)
                {
                    char tag[4];
                    memcpy(&tag[0], &m_tableDirectory[m_tableIndex-1].tag, 4);
                    if(strncmp("pamc", tag, 4) == 0)
                    {
                        //Set the read position via the context
                        //run the parser for the cmap table
                    }
                    else if(strncmp("GISD", tag, 4) == 0)
                    {                        
                        

                        GetContext()->SetPosition(m_tableDirectory[m_tableIndex].offset);
                        Workflow()->PushState(
                            new_ref<TTFGISDHeader>(GetDeserializer(), &m_gisd_header, return_pos)
                                components::serial::StructState<gsid_struct, 0, qor_reflection::detail::fields_count<std::remove_cvref_t<gsid_struct>>()>
                                >(GetDeserializer(), arch::Endian::big, &m_gsid_data));
                        
                    }
                }
*/