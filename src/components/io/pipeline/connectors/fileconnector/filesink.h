// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SINKS_FILE
#define QOR_PP_H_COMPONENTS_PIPELINE_SINKS_FILE

#include "src/framework/data/pipeline/iosink.h"
#include "fileconnector.h"

namespace qor{ namespace io{ namespace components{

    class qor_pp_module_interface_gcc(QOR_FILECONNECTOR) FileSink : public pipeline::iosink<FileConnector>
    {
    public:

        qor_pp_module_interface(QOR_FILECONNECTOR) FileSink();
        qor_pp_module_interface(QOR_FILECONNECTOR) virtual ~FileSink();

    protected:

        virtual size_t WriteBytes(byte* data, size_t bytesToWrite);
    };

}}}//qor::io::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SINKS_FILE
