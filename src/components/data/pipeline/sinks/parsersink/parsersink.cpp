// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "parsersink.h"
#include "src/framework/data/pipeline/source.h"
#include "src/qor/flyers/interception/functioncontext.h"

namespace qor{ namespace pipeline{ namespace components{ 

    BaseParserSink::~BaseParserSink() = default;        

    BaseParserSink::BaseParserSink() : 
        m_parser(){ }
   

    qor::data::Parser& BaseParserSink::Parser()
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
                if(unitsToWrite > 0)
                {
                    /*unitsWritten =*/ Parse();//Parse will read buffer incrementally through context
                    if(unitsWritten > 0)
                    {                        
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
            log::debug("Nothing to Push!");
        }
        return true;
    }

    size_t BaseParserSink::Parse()
    {
        fatal("Empty base called. Please overrride bool Parse(byte*, size_t); in your BaseParserSink derived class.");
        return 0;
    }
}}}//qor::parser::components
