// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCES_FONTRESOURCE
#define QOR_PP_H_FRAMEWORK_RESOURCES_FONTRESOURCE

#include <string>

#include "../../resource.h"
#include "src/framework/io/filesystem/path.h"
#include "src/framework/io/filesystem/fileindex.h"
#include "ttfobject.h"

namespace qor{ namespace framework{ namespace res {

    enum FontType
    {
        Unknown,
        TrueType,
        OpenTypePS,
        PostScriptType1,
    };

    class qor_pp_module_interface(QOR_RESOURCES) Font : public Resource
    {
    public:

        static const char* StaticType();

        Font(ResourceManager* manager, const io::filesystem::Index& index, Resource* batchKey = nullptr) : Resource(manager, batchKey), m_index(index)
        {            
            m_fontType = Unknown;
            Name();
        }
        
        virtual ~Font() = default;

        virtual const char* Type();
        virtual void Name();
        virtual void Locate();
        virtual void Claim();
                
        ref_of<TTFObject>::type GetObject();

    protected:

        FontType m_fontType;
        const io::filesystem::Index m_index;      
        ref_of<TTFObject>::type m_object;  
        
    private:
        static const char* s_fontResourceType;
        
    };

}}}//qor::framework::res

#endif//QOR_PP_H_FRAMEWORK_RESOURCES_FONTRESOURCE