// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <string.h>
#include "smallobjectbucket.h"
#include "../fastheap/fastheap.h"

namespace qor{ namespace memory{

    void* SmallObjectBucket::operator new(size_t sz)
    {
        return new_ref<FastHeap>()->Allocate(sz);
    }

    void SmallObjectBucket::operator delete(void* allocation)
    {
        new_ref<FastHeap>()->Free(reinterpret_cast<byte*>(allocation), sizeof(SmallObjectBucket));
    }

    SmallObjectBucket::SmallObjectBucket(size_t unitSize) : 
        m_unitSize(sizeof(int*) + unitSize)
        , m_lastPage(nullptr)
        , m_allocCount(0)
        , m_pageCache(nullptr)
        , m_cacheCount(0) 
        , m_cacheLimit(0)
    { }

    SmallObjectBucket::~SmallObjectBucket()
    {
        while(m_pageCache)
        {
            FlushOnePageFromCache();
        }
    }

    byte* SmallObjectBucket::Allocate()
    {
        byte* result = nullptr;
        SmallObjectPage* page = m_lastPage;			//Get a page in which to allocate

        if (page == nullptr || page->IsFull())
        {
            page = m_pageCache;
            if (m_pageCache)
            {
                m_pageCache = m_pageCache->m_prev;
                m_cacheCount--;
            }
            if (page == nullptr)
            {
                size_t mapWords = 1 + (32 / m_unitSize) + (m_allocCount / 4096);//use larger pages as the bucket grows
                page = new(new_ref<FastHeap>()->Allocate(sizeof(SmallObjectPage))) SmallObjectPage(m_unitSize, mapWords);	//construct a page object in the allocated memory
                //record this page as the last one we dealt with and return the page
            }
        }

        if (page)
        {
            m_lastPage = page;
            result = page->Allocate();				//Allocate within the page
            if (result)
            {
                m_allocCount++;						//Increment the count of allocations
                *(reinterpret_cast<SmallObjectPage**>(result - sizeof(int*))) = page;//Add a page back pointer to the allocation
            }
        }
        return result;
    }

    bool SmallObjectBucket::Free(byte* element)
    {
        if (element != nullptr)																		//If there's an element to delete
        {
            SmallObjectPage* page = *(reinterpret_cast<SmallObjectPage**>(element - sizeof(int*))); //Get the page from the back pointer
            if (page && page->Free(element) )														//Free the element within the page
            {
                m_allocCount--;																		//Decrement the allocation count
                if (page->IsEmpty())																//If the page is empty
                {
                    if (m_lastPage == page)															//invalidate recent page cache
                    {
                        m_lastPage = page->m_prev;;
                    }
                    FreePage(page);																	//Free the page
                }
            }
            else
            {
                throw memoryexception("Memory to free ar {0:p} not found in Small Object Heap.", (void*)element);
            }
        }        
        return true;
    }

    void SmallObjectBucket::AddOnePageToCache(SmallObjectPage* page)
    {
        if (m_cacheCount < m_cacheLimit)
        {
            page->m_prev = m_pageCache;
            m_pageCache = page;
            m_cacheCount++;
        }
        else
        {   //No room in the cache so dump the page
            page->~SmallObjectPage();
            new_ref<FastHeap>()->Free(page, sizeof(SmallObjectPage));
        }

    }

    void SmallObjectBucket::FlushOnePageFromCache()
    {
        SmallObjectPage* prev = m_pageCache->m_prev;
        m_pageCache->~SmallObjectPage();
        new_ref<FastHeap>()->Free(m_pageCache, sizeof(SmallObjectPage));
        m_pageCache = prev;
        m_cacheCount--;
    }

    void SmallObjectBucket::FreePage(SmallObjectPage* checkPage)
    {
        while (m_pageCache && (m_cacheCount >= m_cacheLimit))
        {
            FlushOnePageFromCache();
        }

        AddOnePageToCache(checkPage);
    }

}}//qor::memory
