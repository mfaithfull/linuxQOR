// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_FILESYSTEM_FILE_READER
#define QOR_PP_H_PLATFORM_FILESYSTEM_FILE_READER

#include <cstdint>
#include "src/qor/reflection/reflection.h"
#include "file.h"
#include "../filesystem.h"

namespace qor{ namespace io{

    class qor_pp_module_interface_gcc(QOR_FILESYSTEM) FileReader
	{
	public:

        qor_pp_module_interface(QOR_FILESYSTEM) FileReader(ref_of<File>::type file);
        qor_pp_module_interface(QOR_FILESYSTEM) FileReader(filesystem::Index index, OpenFor use);
        qor_pp_module_interface(QOR_FILESYSTEM) virtual ~FileReader();

        qor_pp_module_interface(QOR_FILESYSTEM) std::string ReadLine() const;
        qor_pp_module_interface(QOR_FILESYSTEM) byte ReadByte() const;
        qor_pp_module_interface(QOR_FILESYSTEM) char ReadChar() const;
        qor_pp_module_interface(QOR_FILESYSTEM) bool ReadBool() const;// Always 1 byte, no ordering        
        qor_pp_module_interface(QOR_FILESYSTEM) int16_t ReadInt16() const;
        qor_pp_module_interface(QOR_FILESYSTEM) uint16_t ReadUInt16() const;
        qor_pp_module_interface(QOR_FILESYSTEM) int32_t ReadInt32() const;
        qor_pp_module_interface(QOR_FILESYSTEM) uint32_t ReadUInt32() const;
        qor_pp_module_interface(QOR_FILESYSTEM) int64_t ReadInt64() const;
        qor_pp_module_interface(QOR_FILESYSTEM) uint64_t ReadUInt64() const;
        qor_pp_module_interface(QOR_FILESYSTEM) float ReadFloat() const;
        qor_pp_module_interface(QOR_FILESYSTEM) double ReadDouble() const;

        qor_pp_module_interface(QOR_FILESYSTEM) void Read(unsigned char& b) const;
        qor_pp_module_interface(QOR_FILESYSTEM) void Read(char& c) const;
        qor_pp_module_interface(QOR_FILESYSTEM) void Read(bool& b) const;
        qor_pp_module_interface(QOR_FILESYSTEM) void Read(short& s) const;
        qor_pp_module_interface(QOR_FILESYSTEM) void Read(unsigned short& s) const;
        qor_pp_module_interface(QOR_FILESYSTEM) void Read(int& i) const;
        qor_pp_module_interface(QOR_FILESYSTEM) void Read(unsigned int& ui) const;
        qor_pp_module_interface(QOR_FILESYSTEM) void Read(long& l) const;
        qor_pp_module_interface(QOR_FILESYSTEM) void Read(unsigned long& ul) const;
        qor_pp_module_interface(QOR_FILESYSTEM) void Read(long long& ll) const;
        qor_pp_module_interface(QOR_FILESYSTEM) void Read(unsigned long long& ull) const;
        qor_pp_module_interface(QOR_FILESYSTEM) void Read(float& f) const;
        qor_pp_module_interface(QOR_FILESYSTEM) void Read(double& d) const;

		template< class T >
		void ReadStruct(T& t) const
		{
			qor_reflection::for_each_field(t, [this](auto& value, std::size_t i)
				{
                    Read(value);
				}
			);
		}

    private:

        arch::Endian m_endian;
        ref_of<File>::type m_file;
        filesystem::Index m_index;
    };

}}//qor::io

#endif//QOR_PP_H_PLATFORM_FILESYSTEM_FILE_READER
