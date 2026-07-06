// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_STDOFSTREAM
#define QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_STDOFSTREAM

#include <fstream>
#include "src/framework/io/filesystem/fileindex.h"
#include "stdfstreamconnector.h"

namespace qor{ namespace io{ namespace components{ 

    class qor_pp_module_interface(QOR_STDFSTREAMCONNECTOR) stdofstreamConnector : public stdfstreamConnector
    {
    public:

		stdofstreamConnector();
		stdofstreamConnector(io::filesystem::Index& fileIndex);
		virtual ~stdofstreamConnector();

        //Plug interface
        virtual bool Connect();													//Device specific connection
        virtual void Disconnect(void);											//Device specific disconnection

        std::ofstream& Stream();

	protected:

        std::ofstream m_ofstream;
    };

}}}//qor::io::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_STDOFSTREAM
