// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_PLUG
#define QOR_PP_H_PIPELINE_PLUG

#include "src/framework/event/signals/signal.h"
#include "src/framework/parallel/sync/syncobject.h"
#include "src/framework/io/pipeline/element.h"

namespace qor{ namespace pipeline{

    class qor_pp_module_interface(QOR_PIPELINE) Plug : public virtual Element, public SignalBase
    {

    public:

        qor_pp_signal_func ConnectedSignal();
        qor_pp_signal_func ConnectionErrorSignal();
        qor_pp_signal_func DisconnectedSignal();
        qor_pp_signal_func DisconnectionErrorSignal();

        Plug();
        virtual ~Plug();

        virtual void SetSink(Element* /*ignored*/){};
        virtual void SetSource(Element* /*ignored*/){};
        virtual Element* GetSink() const { return nullptr; }
        virtual Element* GetSource() const { return nullptr; }

        virtual bool IsPlug() { return true; }
        virtual bool Connect();
        virtual void Disconnect();
        virtual bool HandlePendingConnectionResult(bool connected);
        virtual void OnConnected();
        virtual void OnConnectionError();
        virtual void OnDisconnected();
        virtual void OnDisconnectionError();
        bool IsConnected() const;
        virtual bool AsyncConnection() const;
        virtual SyncObject* GetSyncObject();
        virtual void SetSyncObject(SyncObject* syncobject);
        virtual bool CheckComplete();
        virtual const char* Name() const override;
        
    protected:

        bool m_connected;
        SyncObject* m_syncobject;
    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_PLUG
