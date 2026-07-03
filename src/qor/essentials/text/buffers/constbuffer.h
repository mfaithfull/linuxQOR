// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_CONSTBUFFER
#define QOR_PP_H_TEXT_CONSTBUFFER

#include <string>
#include "src/qor/essentials/iterators/iterators.h"

namespace qor{

    namespace text{
        qor_pp_module_interface(QOR_TEXT) void OutOfRangeError(size_t index, size_t length, size_t elementSize, const void* bufferAddress);
    }

    template< typename T >
    struct ConstBuffer final
    {
    public:

        typedef rawiterator<const T> iterator;
        typedef rawiterator<const T> const_iterator;
        typedef rawreverseiterator<const T> reverse_iterator;
        typedef rawreverseiterator<const T> const_reverse_iterator;

        class View : public std::basic_string_view< T >
        {
        public:

            constexpr View() noexcept : std::basic_string_view< T >(){ }
            constexpr View(const ConstBuffer<T>& buffer) : std::basic_string_view< T >(buffer.GetData(), buffer.Length()), m_buffer(buffer) { }
            constexpr View(const View& src) : std::basic_string_view< T >(src), m_buffer(src.m_buffer){ }

            constexpr const T* operator -> () const
            {
                return m_buffer.m_p;
            }

            constexpr operator const T*() const
            {
                return m_buffer.m_p;
            }

            constexpr bool IsNotNull() const
            {
                const T* p = operator const T*();
                return p == nullptr ? false : true;
            }

            virtual ~View()
            {
            }

        private:
            const ConstBuffer<T>& m_buffer;
        };

        template<size_t N>
        constexpr ConstBuffer(const T(&str)[N]) : m_p(static_cast<const T*>(str)), m_unitCount(N - 1){ }

        constexpr ConstBuffer( const T* pBuffer, size_t unitCount ) : m_p( pBuffer ), m_unitCount( unitCount )
        {
            if(m_p && m_p[0] == T(0))
            {
                m_unitCount = 0;
            }
        }

        constexpr ConstBuffer(const ConstBuffer& src) : m_p(src.m_p), m_unitCount(src.m_unitCount){ }

        constexpr ConstBuffer(ConstBuffer&& src) noexcept : m_p(src.m_p), m_unitCount(src.m_unitCount){ }

        constexpr ConstBuffer& operator = ( const ConstBuffer& src )
        {
            if( this != &src )
            {
                m_p = src.m_p;
                m_unitCount = src.m_unitCount;
            }
            return *this;
        }

        constexpr View view() const
        {
            return View(*this);
        }

        constexpr size_t GetUnitCount() const
        {
            return m_unitCount;
        }

        constexpr size_t AllocationByteCount() const
        {
            return m_unitCount * sizeof(T);
        }

        constexpr size_t Length() const
        {
            return m_unitCount;
        }

        constexpr size_t size() const //for iterators
        {
            return m_unitCount;
        }

        constexpr size_t ByteLength() const
        {
            return m_unitCount * sizeof(T);
        }

        constexpr bool IsEmpty(void) const
        {
            return m_p == nullptr || m_unitCount == 0 || m_p[0] == T(0);
        }

        constexpr const T* GetData() const
        {
            return m_p;
        }

        constexpr const T* c_str() const
        {
            return m_p;
        }

        constexpr T At(size_t index) const
        {
            if (m_p == nullptr || index >= Length())
            {
                text::OutOfRangeError(index, Length(), sizeof(T), m_p);
            }
            return m_p[index];
        }

        constexpr T operator[](size_t index) const
        {
            if (m_p == nullptr || index >= Length())
            {
                text::OutOfRangeError(index, Length(), sizeof(T), m_p);
            }
            return m_p[index];
        }

        template<typename C = T>
        constexpr const std::basic_string<C> ToStdString() const
        {
            return std::basic_string<C>((const C*)m_p, m_unitCount);
        }

        constexpr operator std::basic_string<T>() const
        {
            return ToStdString();
        }

        constexpr iterator begin() const
        {
            return iterator(m_p);
        }

        constexpr const_iterator cbegin() const
        {
            return const_iterator(m_p);
        }

        constexpr iterator end() const
        {
            return iterator(m_p + m_unitCount);
        }

        constexpr const_iterator cend() const
        {
            return const_iterator(m_p + m_unitCount);
        }

        constexpr reverse_iterator rbegin() const
        {
            return reverse_iterator(m_p + m_unitCount - 1);
        }

        constexpr const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator(m_p + m_unitCount - 1);
        }

        constexpr reverse_iterator rend() const
        {
            return reverse_iterator(m_p - 1);
        }

        constexpr const_reverse_iterator crend() const
        {
            return const_reverse_iterator(m_p - 1);
        }

    private:

        const T* m_p;
        size_t m_unitCount;
    };

}//qor

#endif//QOR_PP_H_TEXT_CONSTBUFFER
