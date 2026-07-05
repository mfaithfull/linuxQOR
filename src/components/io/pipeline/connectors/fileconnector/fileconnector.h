// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_FILE
#define QOR_PP_H_COMPONENTS_PIPELINE_CONNECTIONS_FILE

#include "src/framework/io/pipeline/connection.h"
#include "src/framework/io/filesystem/ifilesystem.h"
#include "src/framework/io/filesystem/fileindex.h"
#include "src/framework/io/filesystem/file/file.h"

namespace qor{ namespace io{ namespace components{

    class qor_pp_module_interface(QOR_FILECONNECTOR) FileConnector : public pipeline::Plug
    {
    public:

		FileConnector();
		FileConnector(const filesystem::Index & fileIndex, const OpenFor openfor = OpenFor::ReadWrite, const int mode = WithFlags::CreateNew);
		FileConnector(const filesystem::Index & fileIndex, const pipeline::Buffer& buffer, const OpenFor openfor = OpenFor::ReadWrite, const int mode = WithFlags::CreateNew);
		FileConnector(const filesystem::Index & fileIndex, pipeline::Buffer& buffer, const OpenFor openfor = OpenFor::ReadWrite, const int mode = WithFlags::CreateNew);
		virtual ~FileConnector();

        //Plug interface
        virtual bool Connect();													//Device specific connection
        virtual void Disconnect();  											//Device specific disconnection
        virtual bool HandlePendingConnectionResult(bool bConnected);			//Handling device specific pending connection states for async connections

    	void SetMode(int mode);
	    int GetMode(void);
		void SetOpenFor(OpenFor openfor);
		OpenFor GetOpenFor(void);
		void SetFile(filesystem::Index & fileIndex);
		filesystem::Index& FileIndex(void);
		ref_of<io::File>::type File(void);
        virtual pipeline::Element* GetSink() const;
        virtual pipeline::Element* GetSource() const;

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
