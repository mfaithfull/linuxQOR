// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_SOCKET
#define QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_SOCKET

#include "src/framework/data/pipeline/iosource.h"
#include "socketconnector.h"

namespace qor{ namespace io{ namespace network{ namespace components{ 

    class qor_pp_module_interface(QOR_SOCKETCONNECTOR) SocketSource : public pipeline::iosource<SocketConnector>
    {
    
    public:

        SocketSource();
        virtual ~SocketSource();

    protected:

        virtual size_t ReadBytes(byte* data, size_t bytesToRead);
       
    };

}}}}//qor::io::network::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_SOCKET
