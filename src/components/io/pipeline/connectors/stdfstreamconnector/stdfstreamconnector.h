// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_STDFSTREAM
#define QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_STDFSTREAM

#include <fstream>
#include "src/framework/io/filesystem/fileindex.h"
#include "src/framework/data/pipeline/plug.h"

namespace qor{ namespace io { namespace components{ 

    class qor_pp_module_interface(QOR_STDFSTREAMCONNECTOR) stdfstreamConnector : public pipeline::Plug
    {
    public:

		stdfstreamConnector();
		stdfstreamConnector(io::filesystem::Index& fileIndex);
		virtual ~stdfstreamConnector();

        //Plug interface
        virtual bool Connect();													//Device specific connection
        virtual void Disconnect(void);											//Device specific disconnection
        virtual void SetFile(io::filesystem::Index& fileIndex);
        std::string GetFileName();
	protected:

        std::string m_filePath;
    };

}}}//qor::io::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_STDFSTREAM
