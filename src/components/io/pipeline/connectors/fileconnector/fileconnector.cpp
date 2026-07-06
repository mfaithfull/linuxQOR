// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "fileconnector.h"
#include "filesink.h"
#include "filesource.h"

namespace qor{ namespace io{ namespace components{ 

    FileConnector::FileConnector() : Plug()
    {
        m_Mode = WithFlags::None;        
        m_OpenFor = OpenFor::ReadWrite;        

        m_sink = new_ref<FileSink>();
        m_source = new_ref<FileSource>();
        m_sink->SetPlug(this);
        m_source->SetPlug(this);
    }

    FileConnector::FileConnector(const filesystem::Index& fileIndex, const OpenFor openfor, const int mode) : Plug()
    {
        m_Mode = mode;
        m_OpenFor = openfor;
        m_pFileIndex = &(const_cast<filesystem::Index&>(fileIndex));

        m_sink = new_ref<FileSink>();
        m_source = new_ref<FileSource>();
        m_sink->SetPlug(this);
        m_source->SetPlug(this);
    }

    FileConnector::FileConnector(const filesystem::Index& fileIndex, const pipeline::Buffer& buffer, const OpenFor openfor, const int mode) : 
        FileConnector(fileIndex, openfor, mode)
    {
        m_source->SetBuffer(buffer);
        m_sink->SetBuffer(buffer);
    }

    FileConnector::FileConnector(const filesystem::Index& fileIndex, pipeline::Buffer& buffer, const OpenFor openfor, const int mode) : FileConnector(fileIndex, openfor, mode)
    {
        m_source->SetBuffer(buffer);
        m_sink->SetBuffer(buffer);
    }

    FileConnector::~FileConnector()
    {
        if (m_connected)
        {
            Disconnect();
        }
    }

    bool FileConnector::Connect()
    {
        if(!m_connected)
        {
            m_File = new_ref<io::File>(*m_pFileIndex, static_cast<int>(m_OpenFor), m_Mode);
            m_connected = !m_File.IsNull();//->IsOpen();
        }
        return m_connected;
    }

    void FileConnector::Disconnect()
    {
        m_File.Dispose();
        m_connected = false;
    }
    
    /*
    bool FileConnector::HandlePendingConnectionResult(bool bConnected)
    {        
        if (bConnected)
        {
            return true;
        }

        return false;
    }*/

    void FileConnector::SetMode(int mode)
    {
        m_Mode = mode;
    }

    int FileConnector::GetMode(void)
    {
        return m_Mode;
    }

    void FileConnector::SetOpenFor(OpenFor openfor)
    {
        m_OpenFor = openfor;
    }

	OpenFor FileConnector::GetOpenFor(void)
    {
        return m_OpenFor;
    }

    void FileConnector::SetFile(filesystem::Index& fileIndex)
    {
        m_pFileIndex = &fileIndex;
    }

    filesystem::Index& FileConnector::FileIndex(void)
    {
        return *m_pFileIndex; 					//The underlying filesystem::Index
    }

    ref_of<io::File>::type FileConnector::File(void)
    {
        return m_File; 							//The underlying File
    }

    pipeline::Element* FileConnector::GetSink() const { return m_sink; }

    pipeline::Element* FileConnector::GetSource() const { return m_source; }

}}}//qor::io::components
