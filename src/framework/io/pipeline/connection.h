// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_CONNECTION
#define QOR_PP_H_PIPELINE_CONNECTION

#include "source.h"
#include "sink.h"
#include "plug.h"

namespace qor{ namespace pipeline{

    class Connection : public Plug
    {
    public:

        Connection() : Plug()
            , m_writable(true)
            , m_readable(true)
            , m_async(false)
        { }

        virtual ~Connection() = default;

        virtual Element* GetSource() const
        {
            return m_source;
        }

        virtual Element* GetSink() const
        {
            return m_sink;
        }

        virtual bool CanRead() const
        {
            return m_readable;
        }

        virtual bool CanWrite() const
        {
            return m_writable;
        }

        virtual bool AsyncConnection() const
        {
            return m_async;
        }

        virtual void SetAsync(bool async)
        {
            m_async = async;
        }

        virtual void OnConnected(void)
        {
            //SetSyncObject(m_syncobject);	//Once connected always use the internal SyncObject for IO        
        }

        virtual void SetReadOnly()
        {
            m_writable = false;
            m_readable = true;
        }

        virtual void SetWriteOnly()
        {
            m_writable = true;
            m_readable = false;
        }

        virtual void SetReadWrite()
        {
            m_readable = true;
            m_writable = true;
        }

    protected:

        Source* ActualSource()
        {
            return dynamic_cast<Source*>(m_source);
        }

        Sink* ActualSink()
        {
            return dynamic_cast<Sink*>(m_sink);
        }

        virtual void OnWriteCompleted(size_t error, size_t unitsWritten)
        {
            auto sink = ActualSink();
            if (sink)
            {
                if (error == 0)
                {
                    sink->OnWriteSuccess(unitsWritten);
                }
                else
                {
                    sink->OnWriteError(unitsWritten, error);
                }
            }
        }

        virtual void OnReadCompleted(size_t error, size_t countUnitsRead)
        {
            auto source = ActualSource();

            if (source)
            {
                if (error == 0)
                {
                    source->OnReadSuccess(countUnitsRead);
                }
                else
                {
                    source->OnReadError(error, countUnitsRead);
                }
            }
        }

        bool m_writable;
        bool m_readable;
        bool m_async;
        Element* m_source{ nullptr };
        Element* m_sink{ nullptr };
    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_CONNECTION
