// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_PLUG
#define QOR_PP_H_PIPELINE_PLUG

#include "src/framework/event/signals/signal.h"
#include "src/framework/parallel/sync/syncobject.h"
#include "src/framework/data/pipeline/element.h"

namespace qor{ namespace pipeline{

    class qor_pp_module_interface_gcc(QOR_PIPELINE) Plug : public virtual Element, public SignalBase
    {
    public:

        qor_pp_module_interface(QOR_PIPELINE) qor_pp_signal_func ConnectedSignal();
        qor_pp_module_interface(QOR_PIPELINE) qor_pp_signal_func ConnectionErrorSignal();
        qor_pp_module_interface(QOR_PIPELINE) qor_pp_signal_func DisconnectedSignal();
        qor_pp_module_interface(QOR_PIPELINE) qor_pp_signal_func DisconnectionErrorSignal();

        qor_pp_module_interface(QOR_PIPELINE) Plug();
        qor_pp_module_interface(QOR_PIPELINE) virtual ~Plug();

        qor_pp_module_interface(QOR_PIPELINE) virtual void SetSink(Element* /*ignored*/);
        qor_pp_module_interface(QOR_PIPELINE) virtual void SetSource(Element* /*ignored*/);
        qor_pp_module_interface(QOR_PIPELINE) virtual Element* GetSink() const;
        qor_pp_module_interface(QOR_PIPELINE) virtual Element* GetSource() const;

        qor_pp_module_interface(QOR_PIPELINE) virtual bool IsPlug();
        qor_pp_module_interface(QOR_PIPELINE) virtual bool Connect();
        qor_pp_module_interface(QOR_PIPELINE) virtual void Disconnect();
        //virtual bool HandlePendingConnectionResult(bool connected);
        qor_pp_module_interface(QOR_PIPELINE) virtual void OnConnected();
        qor_pp_module_interface(QOR_PIPELINE) virtual void OnConnectionError();
        qor_pp_module_interface(QOR_PIPELINE) virtual void OnDisconnected();
        qor_pp_module_interface(QOR_PIPELINE) virtual void OnDisconnectionError();
        qor_pp_module_interface(QOR_PIPELINE) bool IsConnected() const;
        qor_pp_module_interface(QOR_PIPELINE) virtual bool AsyncConnection() const;
        qor_pp_module_interface(QOR_PIPELINE) virtual SyncObject* GetSyncObject();
        qor_pp_module_interface(QOR_PIPELINE) virtual void SetSyncObject(SyncObject* syncobject);
        qor_pp_module_interface(QOR_PIPELINE) virtual bool CheckComplete();
        qor_pp_module_interface(QOR_PIPELINE) virtual const char* Name() const override;
        
    protected:

        bool m_connected;
        SyncObject* m_syncobject;
    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_PLUG
