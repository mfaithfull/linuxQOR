// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_FILESYSTEM_FILE_WRITER
#define QOR_PP_H_PLATFORM_FILESYSTEM_FILE_WRITER

#include <cstdint>
#include "src/qor/reflection/reflection.h"
#include "../filesystem.h"
#include "file.h"

namespace qor{ namespace io{

    template <std::size_t I, std::size_t N> struct StructWriter;

    class qor_pp_module_interface(QOR_FILESYSTEM) FileWriter
	{
	public:

        FileWriter(ref_of<File>::type file);
        FileWriter(filesystem::Index index, OpenFor use);
        virtual ~FileWriter() = default;

        void WriteLine(const std::string& line, const std::string& end = "\n") const;
        void WriteByte(const byte& b) const;
        void WriteChar(const char& c) const;
        void WriteBool(const bool& b) const;
        void WriteInt16(const int16_t& i, arch::Endian endian = arch::network) const;
        void WriteUInt16(const uint16_t& ui, arch::Endian endian = arch::network) const;
        void WriteInt32(const int32_t& i, arch::Endian endian = arch::network) const;
        void WriteUInt32(const uint32_t& ui, arch::Endian endian = arch::network) const;
        void WriteInt64(const int64_t& i, arch::Endian endian = arch::network) const;
        void WriteUInt64(const uint64_t& ui, arch::Endian endian = arch::network) const;
        void WriteFloat(const float& f, arch::Endian endian = arch::network) const;
        void WriteDouble(const double& d, arch::Endian endian = arch::network) const;

        void Write(const char, arch::Endian endian = arch::network) const;
        void Write(const unsigned char, arch::Endian endian = arch::network) const;
        void Write(const bool, arch::Endian endian = arch::network) const;
        void Write(const short, arch::Endian endian = arch::network) const;
        void Write(const unsigned short s, arch::Endian endian = arch::network) const;
        void Write(const int i, arch::Endian endian = arch::network) const;
        void Write(const unsigned int ui, arch::Endian endian = arch::network) const;
        void Write(const long l, arch::Endian endian = arch::network) const;
        void Write(const unsigned long ul, arch::Endian endian = arch::network) const;
        void Write(const long long ll, arch::Endian endian = arch::network) const;
        void Write(const unsigned long long ull, arch::Endian endian = arch::network) const;
        void Write(const float f, arch::Endian endian = arch::network) const;
        void Write(const double d, arch::Endian endian = arch::network) const;

		template< class T >
		void WriteStruct(const T& t, arch::Endian endian = arch::network) const
		{
			constexpr std::size_t fieldCount = qor_reflection::detail::fields_count<T>();
			StructWriter<0, fieldCount>::Write(*this, qor_reflection::detail::tie_as_tuple(t), t, endian);
		}

    private:

        ref_of<File>::type m_file;
        filesystem::Index m_index;
    };

    template <std::size_t I, std::size_t N> 
    struct StructWriter
    {
        template< class T, class R>
        static void Write(FileWriter& writer, const T& value, const R& org_val, arch::Endian endian = arch::network)
        {
            auto val = qor_reflection::detail::sequence_tuple::get<I>(value);
            writer.Write(val, endian);
            StructWriter<I + 1, N>::Write(writer, value, org_val, endian);
        }
    };

    template <std::size_t I>
    struct StructWriter<I, I>
    {
        template <class T, class R> static void Write(FileWriter& container, const T& value, const R& org_val, arch::Endian endian = arch::network) noexcept {}
    };

}}//qor::io

#endif//QOR_PP_H_PLATFORM_FILESYSTEM_FILE_READER
