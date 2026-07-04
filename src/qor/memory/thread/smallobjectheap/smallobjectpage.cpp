// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <string.h>
#include "smallobjectpage.h"
#include "../fastheap/fastheap.h"

namespace qor{ namespace memory{

    SmallObjectPage::SmallObjectPage(size_t unitBytes, size_t mapWordCount) : 
        m_map(nullptr), m_mapWords(mapWordCount), m_unitSize(unitBytes)
    {
        size_t mapBytes = (sizeof(size_t) * m_mapWords);
        m_pageByteCount = mapBytes + (mapBytes * 8 * unitBytes);
        m_map = reinterpret_cast<size_t*>(new_ref<FastHeap>()->Allocate(m_pageByteCount));
        memset(m_map, 0, mapBytes);
        m_memory = reinterpret_cast<byte*>(m_map + m_mapWords);
        m_prev = nullptr;
    }

    SmallObjectPage::~SmallObjectPage()
    {
        new_ref<FastHeap>()->Free(m_map, m_pageByteCount);
        m_map = nullptr;
        m_memory = nullptr;
        m_prev = nullptr;
    }

    byte* SmallObjectPage::Allocate()
    {
        byte* mem = nullptr;
        if(!IsFull())
        {
            for(size_t mapWord = 0; mapWord < m_mapWords; ++mapWord)
            {
                size_t bitIndex = 0;
                size_t searchBit = 1;
                size_t map = m_map[mapWord];
                while(( map & searchBit) != 0 && bitIndex < sc_WordBits)
                {
                    bitIndex++;
                    searchBit = searchBit << 1;
                }

                if(bitIndex < sc_WordBits)
                {
                    m_map[mapWord] |= searchBit;
                    mem = m_memory + sizeof(int*) + (mapWord * (sc_WordBits * m_unitSize)) + (bitIndex * m_unitSize);
                    break;
                }
            }
        }
        return mem;
    }

    bool SmallObjectPage::IsFull() const
    {
        for (size_t mapWord = 0; mapWord < m_mapWords; mapWord++)
        {
            if ((~m_map[mapWord] & ((size_t)-1)) != 0)
            {
                return false;
            }
        }
        return true;
    }

    bool SmallObjectPage::IsEmpty() const
    {
        for (size_t mapWord = 0; mapWord < m_mapWords; mapWord++)
        {
            if (m_map[mapWord] != 0)
            {
                return false;
            }
        }
        return true;
    }

    bool SmallObjectPage::Free(byte* element)
    {
        bool bResult = false;
        size_t mapBytes = (sizeof(size_t) * m_mapWords);
        element -= sizeof(size_t*);
        if ((element >= m_memory) && (element < (m_memory + (mapBytes * 8 * m_unitSize))))
        {
            byte* pMem = reinterpret_cast<byte*>(element);
            size_t index = (pMem - m_memory) / m_unitSize;
            size_t wordIndex = index / sc_WordBits;
            size_t bitIndex = index - (wordIndex * sc_WordBits);
            Free(wordIndex, bitIndex);
            bResult = true;
        }
        else
        {
            bResult = false;
        }
        return bResult;
    }

}}//qor::memory
