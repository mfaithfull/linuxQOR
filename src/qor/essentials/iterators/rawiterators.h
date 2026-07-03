// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ITERATORS_RAW
#define QOR_PP_H_ITERATORS_RAW

#include<iterator>
#include<memory>

namespace qor
{
    template<typename _Category, typename _Tp, typename _Distance = ptrdiff_t, typename _Pointer = _Tp*, typename _Reference = _Tp&>
    struct iterator_base
    {
        typedef _Category  iterator_category; // One of the iterator_tags tag types link.
        typedef _Tp        value_type;        // The type "pointed to" by the iterator.
        typedef _Distance  difference_type;   // Distance between iterators is represented as this type.      
        typedef _Pointer   pointer;           // This type represents a pointer-to-value_type.      
        typedef _Reference reference;         // This type represents a reference-to-value_type.
    };

    template<typename dataT>
    class rawiterator : public iterator_base<std::bidirectional_iterator_tag, dataT, ptrdiff_t, dataT*, dataT&>
    {
    public:

        rawiterator(dataT* ptr = nullptr){m_ptr = ptr;}
        rawiterator(const rawiterator<dataT>& rawIterator) = default;
        virtual ~rawiterator(){}

        rawiterator<dataT>& operator=(const rawiterator<dataT>& rawIterator) = default;
        rawiterator<dataT>& operator=(dataT* ptr){m_ptr = ptr;return (*this);}

        operator bool()const
        {
            return m_ptr ? true : false;
        }

        bool operator==(const rawiterator<dataT>& rawIterator)const{return (m_ptr == rawIterator.getConstPtr());}
        bool operator!=(const rawiterator<dataT>& rawIterator)const{return (m_ptr != rawIterator.getConstPtr());}

        rawiterator<dataT>& operator+=(const ptrdiff_t& movement){m_ptr += movement;return (*this);}
        rawiterator<dataT>& operator-=(const ptrdiff_t& movement){m_ptr -= movement;return (*this);}
        rawiterator<dataT>& operator++(){++m_ptr;return (*this);}
        rawiterator<dataT>& operator--(){--m_ptr;return (*this);}
        rawiterator<dataT> operator++(int){auto temp(*this);++m_ptr;return temp;}
        rawiterator<dataT> operator--(int){auto temp(*this);--m_ptr;return temp;}
        rawiterator<dataT> operator+(const ptrdiff_t& movement){auto oldPtr = m_ptr;m_ptr+=movement;auto temp(*this);m_ptr = oldPtr;return temp;}
        rawiterator<dataT> operator-(const ptrdiff_t& movement){auto oldPtr = m_ptr;m_ptr-=movement;auto temp(*this);m_ptr = oldPtr;return temp;}

        ptrdiff_t operator-(const rawiterator<dataT>& rawIterator){return std::distance(rawIterator.getPtr(),this->getPtr());}

        dataT& operator*() const noexcept
        {
            return *m_ptr;
        }
        //const dataT& operator*()const{return *m_ptr;}
        dataT* operator->() const noexcept
        {
            return m_ptr;
        }

        operator dataT*() const noexcept    //It's important to be able to decay iterators to pointers
        {
            return m_ptr;
        }

        dataT* getPtr()const{return m_ptr;}
        const dataT* getConstPtr()const{return m_ptr;}

    protected:

        dataT*                                 m_ptr;
    };

    // Raw reverse iterator with bidrectional access
    template<typename dataT>
    class rawreverseiterator : public rawiterator<dataT>
    {
    public:

        rawreverseiterator(dataT* ptr = nullptr):rawiterator<dataT>(ptr){}
        rawreverseiterator(const rawiterator<dataT>& rawIterator){this->m_ptr = rawIterator.getPtr();}
        rawreverseiterator(const rawreverseiterator<dataT>& rawReverseIterator) = default;
        virtual ~rawreverseiterator(){}

        rawreverseiterator<dataT>& operator=(const rawreverseiterator<dataT>& rawReverseIterator) = default;
        rawreverseiterator<dataT>& operator=(const rawiterator<dataT>& rawIterator){this->m_ptr = rawIterator.getPtr();return (*this);}
        rawreverseiterator<dataT>& operator=(dataT* ptr){this->setPtr(ptr);return (*this);}

        rawreverseiterator<dataT>& operator+=(const ptrdiff_t& movement){this->m_ptr -= movement;return (*this);}
        rawreverseiterator<dataT>& operator-=(const ptrdiff_t& movement){this->m_ptr += movement;return (*this);}
        rawreverseiterator<dataT>& operator++(){--this->m_ptr;return (*this);}
        rawreverseiterator<dataT>& operator--(){++this->m_ptr;return (*this);}
        rawreverseiterator<dataT> operator++(int){auto temp(*this);--this->m_ptr;return temp;}
        rawreverseiterator<dataT> operator--(int){auto temp(*this);++this->m_ptr;return temp;}
        rawreverseiterator<dataT> operator+(const int& movement){auto oldPtr = this->m_ptr;this->m_ptr-=movement;auto temp(*this);this->m_ptr = oldPtr;return temp;}
        rawreverseiterator<dataT> operator-(const int& movement){auto oldPtr = this->m_ptr;this->m_ptr+=movement;auto temp(*this);this->m_ptr = oldPtr;return temp;}

        ptrdiff_t operator-(const rawreverseiterator<dataT>& rawReverseIterator){return std::distance(this->getPtr(),rawReverseIterator.getPtr());}

        rawiterator<dataT> base(){rawiterator<dataT> forwardIterator(this->m_ptr); ++forwardIterator; return forwardIterator;}
    };
}

#endif//QOR_PP_H_ITERATORS_RAW