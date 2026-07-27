// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_PIPE
#define QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_PIPE

#include "src/qor/essentials/current/currentthread.h"
#include "src/framework/io/network/socket.h"
#include "src/framework/io/network/sockets.h"
#include "src/framework/io/filesystem/pipe/pipe.h"
#include "src/framework/data/pipeline/connection.h"
#include "src/framework/io/async/context.h"

namespace qor{ namespace io{ namespace components{

    class qor_pp_module_interface_gcc(QOR_PIPECONNECTOR) PipeConnector : public pipeline::Plug
    {
    public:

		qor_pp_module_interface(QOR_PIPECONNECTOR) PipeConnector();        
		qor_pp_module_interface(QOR_PIPECONNECTOR) virtual ~PipeConnector() noexcept;

        //Plug interface
        qor_pp_module_interface(QOR_PIPECONNECTOR) virtual bool Connect();													//Device specific connection
        qor_pp_module_interface(QOR_PIPECONNECTOR) virtual void Disconnect(void);											//Device specific disconnection
        qor_pp_module_interface(QOR_PIPECONNECTOR) virtual qor::pipeline::Element* GetSink() const;
        qor_pp_module_interface(QOR_PIPECONNECTOR) virtual qor::pipeline::Element* GetSource() const;
        qor_pp_module_interface(QOR_PIPECONNECTOR) void Configure(const std::string &name);
        qor_pp_module_interface(QOR_FILECONNECTOR) ref_of<io::Pipe>::type Pipe(void);

        bool ConnectToName(const std::string name);

	protected:

        std::string m_name;
        qor::ref_of<qor::pipeline::Sink>::type m_sink;
        qor::ref_of<qor::pipeline::Source>::type m_source;
        ref_of<async::Context::Session>::type m_Session;

    private:

        ref_of<io::Pipe>::type m_pipe;

    };

}}}//qor::io::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_PIPE
