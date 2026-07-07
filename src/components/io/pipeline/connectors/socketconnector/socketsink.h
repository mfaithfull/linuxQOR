// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SINKS_SOCKET
#define QOR_PP_H_COMPONENTS_PIPELINE_SINKS_SOCKET

#include "src/framework/data/pipeline/iosink.h"
#include "socketconnector.h"

namespace qor{ namespace io{ namespace network{ namespace components{ 

    class qor_pp_module_interface_gcc(QOR_SOCKETCONNECTOR) SocketSink : public pipeline::iosink<SocketConnector>
    {
    public:

        qor_pp_module_interface(QOR_SOCKETCONNECTOR) SocketSink();
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual ~SocketSink();

    protected:
    
        qor_pp_module_interface(QOR_SOCKETCONNECTOR) virtual size_t WriteBytes(byte* data, size_t bytesToWrite);
    };

}}}}//qor::io::network::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STDOFSTREAM
