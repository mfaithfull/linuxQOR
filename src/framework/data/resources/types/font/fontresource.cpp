// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/framework/io/filesystem/fileindex.h"
#include "src/framework/io/filesystem/folder.h"
#include "fontresource.h"
#include "reader.h"
#include "src/components/io/pipeline/connectors/fileconnector/fileconnector.h"

namespace qor{ namespace framework{ namespace res {

    const char* Font::s_fontResourceType = "Font resource type";

    const char* Font::StaticType(){ return s_fontResourceType; }

    const char* Font::Type()
    {
        return s_fontResourceType;
    }
    
    void Font::Name()
    {        
        std::string name = m_index.ToString();
        m_URI = "font://<ip-address>/" + name;
        Resource::Name();
    }

    void Font::Locate()
    {        
        //Read head of file and check it is in fact a TTF container
        TTFReader reader;

        auto result = reader.ReadDesignator(
            qor::io::components::FileConnector(
                m_index,
                reader.Buffer(),
                qor::io::OpenFor::ReadOnly,
                qor::io::WithFlags::None                
            )
        );        

        if(result == 0x00010000 || result == 0x74727565) //TFF markers
        {            
            m_fontType = TrueType;
            Resource::Locate();
        }
        else if(result == 0x4F54544F) //OpenType with PostScript outlines
        {
            m_fontType = OpenTypePS;
            Resource::Locate();            
        }
        else if(result == 0x74797031) //Old Type1 PostScript font
        {
            m_fontType = PostScriptType1;
            Resource::Locate();
        }
        else
        {
            //Unknown type. Maybe not a font at all.
            //Treat as blob
        }
        
        return;
    }

    void Font::Claim()
    {        
        //Parse the file fully and extract the TTFObject(s) from it.

        if(m_fontType == TrueType)
        {
            TTFReader reader;

            auto result = reader(
                qor::io::components::FileConnector(m_index, reader.Buffer(), io::OpenFor::ReadOnly, io::WithFlags::None)
            );        

            Resource::Claim();
        }
    }

    ref_of<TTFObject>::type Font::GetObject()
    {
        return m_object;
    }

}}}//qor::framework::res