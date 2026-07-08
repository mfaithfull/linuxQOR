// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCES_TTF_OBJECT
#define QOR_PP_H_FRAMEWORK_RESOURCES_TTF_OBJECT

//What you get by loading a TTF file.

namespace qor{ namespace framework{ namespace res { 

    class qor_pp_module_interface(QOR_RESOURCES) TTFObject
    {
    public:

        TTFObject() = default;
        TTFObject(const TTFObject& src){}
        virtual ~TTFObject() = default;
    };

}}}//qor::framework::res

#endif//QOR_PP_H_FRAMEWORK_RESOURCES_TTF_OBJECT
