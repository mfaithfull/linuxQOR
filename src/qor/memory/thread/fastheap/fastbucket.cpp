// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <string.h>
#include "fastbucket.h"

namespace qor{ namespace memory{

    FastBucket::FastBucket(size_t pageUnits) : m_pageUnits(pageUnits)
    , m_basePage(nullptr), m_ToSPage(nullptr), m_items(0), m_pages(0) { }

    FastBucket::~FastBucket()
    {
        StackPage* page = m_ToSPage;
        while (page)
        {
            m_ToSPage = page->Previous();
            delete page;
            page = m_ToSPage;
        }
    }

    size_t FastBucket::AllocatedSpace() const
    {
        size_t Space = 0;
        StackPage* page = m_basePage;
        while (page)
        {
            Space += page->TotalSizeBytes();
            page = page->Next();
        }
        return Space;
    }

    void FastBucket::Initialise()
    {
        if (m_ToSPage == nullptr)
        {
            m_ToSPage = m_basePage = new StackPage(m_pageUnits);
            m_ToSPage->Initialise();
            m_pages = 1;
        }
    }

    void FastBucket::PushPage(void)
    {
        m_ToSPage->SetNext(new StackPage(m_pageUnits));
        m_ToSPage->Next()->SetPrevious(m_ToSPage);
        m_ToSPage = m_ToSPage->Next();
        m_ToSPage->Initialise();
        m_pages++;
    }

    void FastBucket::PopPage(void)
    {
        m_ToSPage = m_ToSPage->Previous();
        m_ToSPage->Next()->SetPrevious(nullptr);
        delete m_ToSPage->Next();
        m_ToSPage->SetNext(nullptr);
        m_pages--;
    }

    void* FastBucket::Allocate(size_t byteCount)
    {        
        void* mem = m_ToSPage->Allocate(byteCount);

        if (mem == nullptr && (byteCount < (m_pageUnits * StackPage::c_pageSize)))
        {
            PushPage();
            mem = m_ToSPage->Allocate(byteCount);

            if(mem == nullptr)
            {
                mem = new_ref<ThreadHeap>()->Allocate(byteCount);
            }
        }

        if (mem != nullptr)
        {
            m_items++;
        }
        return mem;
    }

    bool FastBucket::Free(void* memory, size_t byteCount)
    {
        //Shortcut to page
        StackPage* page = *(reinterpret_cast< StackPage** >((reinterpret_cast< unsigned char* >(memory)) - sizeof(StackPage**)));

        if (page && page->Free(memory, byteCount) )
        {
            --m_items;
            if (page->AllocatedItemsCount() == 0 && m_pages > 1 )
            {
                StackPage* prev = page->Previous();
                StackPage* next = page->Next();

                if (page == m_ToSPage)
                {
                    m_ToSPage = m_ToSPage->Previous();
                    if (m_ToSPage)
                    {
                        m_ToSPage->SetNext(nullptr);
                    }
                }
                else if (page == m_basePage)
                {
                    m_basePage = m_basePage->Next();
                    if (m_basePage != nullptr)
                    {
                        m_basePage->SetPrevious(nullptr);
                    }
                }
                else
                {
                    if (prev)
                    {
                        prev->SetNext(next);
                        page->SetPrevious(nullptr);
                    }
                    if (next)
                    {
                        next->SetPrevious(prev);
                        page->SetNext(nullptr);
                    }
                }
                delete page;
                m_pages--;
            }
            if (m_pages == 0)
            {
                m_ToSPage = nullptr;
                m_basePage = nullptr;
            }
            return true;
        }
        else
        {
            throw memoryexception("Memory to free at {0:p} not found in Fast Heap.", (void*)memory);
        }
    }

}}//qor::memory

