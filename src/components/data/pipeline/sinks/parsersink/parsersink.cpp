// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "parsersink.h"
#include "src/framework/data/pipeline/source.h"
#include "src/qor/flyers/interception/functioncontext.h"

namespace qor{ namespace pipeline{ namespace components{ 

    BaseParserSink::~BaseParserSink() = default;        

    BaseParserSink::BaseParserSink() : 
        m_context(new_ref<qor::components::parser::Context>()),
        m_parser(m_context){ }
   

    qor::components::parser::Parser& BaseParserSink::Parser()
    {
        return m_parser;
    }

    void BaseParserSink::SetEOD()
    {
        m_EOD = true;
    }

    bool BaseParserSink::Push(size_t& unitsWritten, size_t unitsToWrite)
    {
        qor_pp_ofcontext;
        if(unitsToWrite > 0)
        {
            pipeline::Buffer* buffer = GetBuffer();
            if(buffer)
            {
                byte* data = buffer->ReadRequest(unitsToWrite);
                if(unitsToWrite > 0)
                {
                    //std::string strData((const char*)(data), std::min(buffer->GetUnitSize() * unitsToWrite, 40ull));
                    //log::debug(strData);
                    size_t bytesParsed = Parse(data, buffer->GetUnitSize() * unitsToWrite);
                    if(bytesParsed > 0)
                    {
                        unitsWritten = bytesParsed / buffer->GetUnitSize();
                        buffer->ReadAcknowledge(unitsWritten);
                        OnWriteSuccess(unitsWritten);
                    }
                    else
                    {
                        log::debug("Parse did not consume any data?");
                    }
                }
                else
                {
                    SetEOD();
                    return false;
                }
                return true;
            }
            else
            {
                serious("No buffer on Pipeline Sink.");
            }
            return false;
        }
        else
        {
            log::debug("Pushed 0 units.");
        }
        return true;
    }

    size_t BaseParserSink::Parse(byte* data, size_t bytesToParse)
    {
        fatal("Empty base called. Please overrride bool Parse(byte*, size_t); in your BaseParserSink derived class.");
    }
}}}//qor::parser::components
