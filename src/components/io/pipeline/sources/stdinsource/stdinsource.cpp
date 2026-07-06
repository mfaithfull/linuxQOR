// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <stdio.h>
#include <iostream>
#include "stdinsource.h"
#include "src/framework/data/pipeline/sink.h"
#include "src/qor/flyers/error/error.h"

namespace qor{ namespace io{ namespace components{ 

    StdInSource::StdInSource() : m_lineMode(true){ }
    StdInSource::~StdInSource() = default;

    bool StdInSource::LineMode(bool newMode)
    {
        bool result = m_lineMode;
        m_lineMode = newMode;
        return result;
    }

	string_t StdInSource::ReadLine(size_t& unitsRead, size_t unitsToRead)
	{
        string_t result;
        std::istream::char_type c = 0;
        if(unitsToRead > 0)
        {
            do
            {
                std::cin.read(&c, 1);
                if (!std::cin.good()) 
                {
                    serious("std::cin bad status.");
                    break;
                }
                else
                {
                    result.push_back(c);
                }
            } while (c != 0x0A && --unitsToRead > 0);
                    
            if (!result.empty() && result.back() == 0x0A) 
            {
                result.resize(result.size() - 1);
            }
            unitsRead = result.size();
        }
		return result;
	}

    bool StdInSource::Pull(size_t& unitsRead, size_t unitsToRead)
    {
        if(!unitsToRead)
        {
            return true;
        }
        pipeline::Buffer* buffer = GetBuffer();
        if(buffer)
        {
            byte* space = GetBuffer()->WriteRequest(unitsToRead);
            if(!space)
            {
                continuable("Pipeline stall. No space in source buffer.");
                return false;
            }
            if(LineMode())
            {                
                std::string input = ReadLine(unitsRead,unitsToRead);
                memcpy(space, input.data(), unitsRead);
            }
            else
            {
                unitsRead = fread(space, GetBuffer()->GetUnitSize(), unitsToRead, stdin);
            }

            if(unitsRead > 0)
            {
                buffer->WriteAcknowledge(unitsRead);
                OnReadSuccess(unitsRead);
            }
            else
            {
                OnEndOfData();
            }
            return true;
        }
        return false;
    }
    
}}}//qor::io::components
