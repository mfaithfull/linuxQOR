// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_STDIFSTREAM
#define QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_STDIFSTREAM

#include "stdfstreamconnector.h"
#include "src/framework/io/filesystem/fileindex.h"
#include "src/framework/data/pipeline/plug.h"

namespace qor{ namespace io { namespace components{ 

    class qor_pp_module_interface(QOR_STDFSTREAMCONNECTOR) stdifstreamConnector : public stdfstreamConnector
    {
    public:

		stdifstreamConnector();
		stdifstreamConnector(io::filesystem::Index& fileIndex);
		virtual ~stdifstreamConnector();

        virtual bool Connect();													//Device specific connection
        virtual void Disconnect(void);											//Device specific disconnection

        std::ifstream& Stream();
        
	protected:

        std::ifstream m_ifstream;
    };

}}}//qor::io::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_STDIFSTREAM
