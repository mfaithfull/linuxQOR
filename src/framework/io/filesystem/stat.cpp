// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <filesystem>
#include "stat.h"

namespace qor{ namespace io{ namespace filesystem {
    
    Stat::Stat() = default;
    Stat::~Stat() = default;
    
    Stat::Stat(const Index& index) : m_index(index){ }
    
    bool Stat::IsBlockFile()
    {
        return std::filesystem::is_block_file(m_index.GetPath());
    }

    bool Stat::IsCharacterFile()
    {
        return std::filesystem::is_character_file(m_index.GetPath());
    }

    bool Stat::IsDir()
    {
        return std::filesystem::is_directory(m_index.GetPath());
    }

    bool Stat::IsEmpty()
    {
        return std::filesystem::is_empty(m_index.GetPath());
    }

    bool Stat::IsFIFO()
    {
        return std::filesystem::is_fifo(m_index.GetPath());
    }

    bool Stat::IsOther()
    {
        return std::filesystem::is_other(m_index.GetPath());
    }

    bool Stat::IsRegularFile()
    {
        return std::filesystem::is_regular_file(m_index.GetPath());
    }

    bool Stat::IsSocket()
    {
        return std::filesystem::is_socket(m_index.GetPath());
    }

    bool Stat::IsSymLink()
    {
        return std::filesystem::is_symlink(m_index.GetPath());
    }

    bool Stat::IsValid() 
    { 
        return std::filesystem::exists(m_index.GetPath());
    }

    bool Stat::IsFile() 
    { 
        return std::filesystem::is_regular_file(m_index.GetPath());
    }    

}}}//qor::io::filesystem
