// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SINKS_DATA
#define QOR_PP_H_COMPONENTS_PIPELINE_SINKS_DATA

#include <string>
#include <type_traits>
#include "src/framework/data/pipeline/podbuffer.h"
#include "src/framework/data/pipeline/sink.h"
#include "src/framework/data/pipeline/source.h"
#include "src/platform/compiler/compiler.h"
#include "src/platform/architecture/cpu.h"
#include "src/framework/event/delegate/delegate.h"

namespace qor{ namespace pipeline {

    class qor_pp_module_interface_gcc(QOR_DATASINK) DataSink : public qor::pipeline::Sink
    {
    public:

        qor_pp_module_interface(QOR_DATASINK) DataSink();
        qor_pp_module_interface(QOR_DATASINK) virtual ~DataSink();
        qor_pp_module_interface(QOR_DATASINK) void SetCallback(Delegate<bool(size_t&, size_t)>& callback);
        qor_pp_module_interface(QOR_DATASINK) void SetByteOrder(const arch::Endian byteOrder);
        qor_pp_module_interface(QOR_DATASINK) arch::Endian GetByteOrder();

        template <typename T> requires std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T>
        inline bool Extract(T& value)
        {
            byte* space = reinterpret_cast<byte*>(&value);
            return Extract(space, sizeof(T));
        }

        qor_pp_module_interface(QOR_DATASINK) bool Extract(byte* data, size_t byteCount);

        protected:

        qor_pp_module_interface(QOR_DATASINK) virtual bool Push(size_t& unitsRead, size_t unitsToRead);
        qor_pp_module_interface(QOR_DATASINK) void ExtractBufferValue(size_t size, byte* space, byte* data, size_t totalRead, size_t realRead);
        qor_pp_module_interface(QOR_DATASINK) bool ExtractBufferData(size_t size, byte* space, size_t byteCount, size_t& totalRead);

        arch::Endian m_inputByteOrder{arch::Endian::network};
        Delegate<bool(size_t&, size_t)> m_callBack;
    };

}}//qor::pipeline

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SINKS_DATA
