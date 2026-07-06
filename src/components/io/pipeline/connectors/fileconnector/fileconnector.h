// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_FILE
#define QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_FILE

#include "src/framework/data/pipeline/connection.h"
#include "src/framework/io/filesystem/ifilesystem.h"
#include "src/framework/io/filesystem/fileindex.h"
#include "src/framework/io/filesystem/file/file.h"

namespace qor{ namespace io{ namespace components{

    class qor_pp_module_interface_gcc(QOR_FILECONNECTOR) FileConnector : public pipeline::Plug
    {
    public:

		qor_pp_module_interface(QOR_FILECONNECTOR) FileConnector();
		qor_pp_module_interface(QOR_FILECONNECTOR) FileConnector(const filesystem::Index & fileIndex, const OpenFor openfor = OpenFor::ReadWrite, const int mode = WithFlags::CreateNew);
		qor_pp_module_interface(QOR_FILECONNECTOR) FileConnector(const filesystem::Index & fileIndex, const pipeline::Buffer& buffer, const OpenFor openfor = OpenFor::ReadWrite, const int mode = WithFlags::CreateNew);
		qor_pp_module_interface(QOR_FILECONNECTOR) FileConnector(const filesystem::Index & fileIndex, pipeline::Buffer& buffer, const OpenFor openfor = OpenFor::ReadWrite, const int mode = WithFlags::CreateNew);
		qor_pp_module_interface(QOR_FILECONNECTOR) virtual ~FileConnector();

        //Plug interface
        qor_pp_module_interface(QOR_FILECONNECTOR) virtual bool Connect();		//Device specific connection
        qor_pp_module_interface(QOR_FILECONNECTOR) virtual void Disconnect();  	//Device specific disconnection
        //virtual bool HandlePendingConnectionResult(bool bConnected);			//Handling device specific pending connection states for async connections

    	qor_pp_module_interface(QOR_FILECONNECTOR) void SetMode(int mode);
	    qor_pp_module_interface(QOR_FILECONNECTOR) int GetMode(void);
		qor_pp_module_interface(QOR_FILECONNECTOR) void SetOpenFor(OpenFor openfor);
		qor_pp_module_interface(QOR_FILECONNECTOR) OpenFor GetOpenFor(void);
		qor_pp_module_interface(QOR_FILECONNECTOR) void SetFile(filesystem::Index & fileIndex);
		qor_pp_module_interface(QOR_FILECONNECTOR) filesystem::Index& FileIndex(void);
		qor_pp_module_interface(QOR_FILECONNECTOR) ref_of<io::File>::type File(void);
        qor_pp_module_interface(QOR_FILECONNECTOR) virtual pipeline::Element* GetSink() const;
        qor_pp_module_interface(QOR_FILECONNECTOR) virtual pipeline::Element* GetSource() const;

	protected:

        filesystem::Index* m_pFileIndex;
        int m_Mode;
        OpenFor m_OpenFor;
        ref_of<pipeline::Sink>::type m_sink;
        ref_of<pipeline::Source>::type m_source;

    private:

        ref_of<io::File>::type m_File;

    };

}}}//qor::io::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_FILE
