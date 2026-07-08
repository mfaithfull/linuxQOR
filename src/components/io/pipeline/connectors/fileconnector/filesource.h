// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_FILE
#define QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_FILE

#include "src/framework/data/pipeline/iosource.h"
#include "fileconnector.h"

namespace qor{ namespace io{ namespace components{ 

    class qor_pp_module_interface_gcc(QOR_FILECONNECTOR) FileSource : public pipeline::iosource<FileConnector>
    {    
    public:

        qor_pp_module_interface(QOR_FILECONNECTOR) FileSource();
        qor_pp_module_interface(QOR_FILECONNECTOR) virtual ~FileSource();

    protected:

        virtual size_t ReadBytes(byte* data, size_t bytesToRead);
       
    };

}}}//qor::io:components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_FILE
