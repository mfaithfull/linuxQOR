// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_DATA
#define QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_DATA

#include <string>
#include <type_traits>
#include "src/framework/data/pipeline/podbuffer.h"
#include "src/framework/data/pipeline/source.h"
#include "src/framework/data/pipeline/sink.h"
#include "src/platform/compiler/compiler.h"
#include "src/platform/architecture/cpu.h"
#include "src/framework/event/delegate/delegate.h"

namespace qor{ namespace pipeline{

    class qor_pp_module_interface_gcc(QOR_DATASOURCE) DataSource : public pipeline::Source
    {

    public:

        qor_pp_module_interface(QOR_DATASOURCE) DataSource();
        qor_pp_module_interface(QOR_DATASOURCE) virtual ~DataSource();
        qor_pp_module_interface(QOR_DATASOURCE) void SetCallback( const Delegate<bool(size_t&, size_t)>& callback);
        qor_pp_module_interface(QOR_DATASOURCE) void SetByteOrder(const arch::Endian byteOrder);
        qor_pp_module_interface(QOR_DATASOURCE) void InsertBufferValue(byte* space, size_t realWrite, byte* data, size_t size, size_t totalWritten);
        qor_pp_module_interface(QOR_DATASOURCE) bool Insert(byte* data, size_t byteCount);

        template <typename T> requires std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T>
        inline bool Insert(T value)
        {
            byte* data = reinterpret_cast<byte*>(&value);
            size_t byteCount = sizeof(T);
            return Insert(data, byteCount);
        }
    
    protected:
            
        qor_pp_module_interface(QOR_DATASOURCE) bool Pull(size_t& unitsRead, size_t unitsToRead);

        arch::Endian m_outputByteOrder{arch::Endian::network};
        Delegate<bool(size_t&, size_t)> m_callBack;
    };

}}//qor::pipeline

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_DATA
