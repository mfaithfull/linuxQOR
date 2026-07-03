// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ITERATORS_SMART
#define QOR_PP_H_ITERATORS_SMART

namespace qor{

    template<typename containerT, typename AdvanceDistanceFunctorT, typename BeginEndFunctorT>
    class Iterator : public iterator_base< std::random_access_iterator_tag,
                                        typename std::iterator_traits<decltype(BeginEndFunctorT::begin(std::declval<containerT&>()))>::value_type,
                                        typename std::iterator_traits<decltype(BeginEndFunctorT::begin(std::declval<containerT&>()))>::difference_type,
                                        typename std::iterator_traits<decltype(BeginEndFunctorT::begin(std::declval<containerT&>()))>::pointer,
                                        typename std::iterator_traits<decltype(BeginEndFunctorT::begin(std::declval<containerT&>()))>::reference >
    {
    public:

        typedef containerT* containerPtrT;
        typedef typename std::iterator_traits<typename containerT::iterator>::value_type dataT;
        typedef typename std::iterator_traits<typename containerT::iterator>::pointer dataTPtr;
        typedef typename std::iterator_traits<typename containerT::iterator>::reference dataTRef;
        typedef decltype(BeginEndFunctorT::begin(std::declval<containerT&>())) iterator;

    private:

        iterator m_ptr;
        containerPtrT m_containerPtr{ nullptr };

    public:

        Iterator() = default;

        Iterator(containerT& container)
        {
            m_containerPtr = &container;

            if(m_containerPtr)
                m_ptr = BeginEndFunctorT::begin(*m_containerPtr);
        }

        Iterator(const containerT& container)
        {
            m_containerPtr = &(const_cast<containerT&>(container));

            if(m_containerPtr)
                m_ptr = BeginEndFunctorT::begin(*m_containerPtr);
        }

        Iterator(const containerT* containerPtr)
        {
            m_containerPtr = const_cast<containerT*>(containerPtr);

            if(m_containerPtr)
                m_ptr = BeginEndFunctorT::begin(*m_containerPtr);
        }

        Iterator(const containerPtrT& containerPtr)
        {
            m_containerPtr = containerPtr;

            if(m_containerPtr)
                m_ptr = BeginEndFunctorT::begin(*m_containerPtr);
        }

        Iterator(const iterator& ptr, const containerPtrT& containerPtr)
        {
            m_ptr = ptr;
            m_containerPtr = containerPtr;
        }

        Iterator(const Iterator<containerT,AdvanceDistanceFunctorT,BeginEndFunctorT>&) = default;
        Iterator(Iterator<containerT,AdvanceDistanceFunctorT,BeginEndFunctorT>&&) = default;
        ~Iterator() = default;
        Iterator<containerT, AdvanceDistanceFunctorT,BeginEndFunctorT>& operator=(const Iterator<containerT,AdvanceDistanceFunctorT,BeginEndFunctorT>& iterator) = default;

        dataTRef operator*(){return (*m_ptr);}
        const dataTRef operator*()const{return (*m_ptr);}
        iterator operator->(){return m_ptr;}
        operator const dataT*() const noexcept {return (m_ptr);}//allow decay to underlying iterator

        bool operator==(const Iterator<containerT,AdvanceDistanceFunctorT,BeginEndFunctorT>& iterator) const
        {
            return ( (m_ptr == iterator.getPtr()) && (m_containerPtr == iterator.getContainerPtr()) );
        }

        bool operator!=(const Iterator<containerT,AdvanceDistanceFunctorT,BeginEndFunctorT>& iterator) const
        {
            return ( (m_ptr != iterator.getPtr()) || (m_containerPtr != iterator.getContainerPtr()) );
        }

        // Bool operator so that this iterator can be used in if statements
        explicit operator bool()const
        {
            return this->m_containerPtr ? true : false;
        }

        Iterator<containerT, AdvanceDistanceFunctorT, BeginEndFunctorT>& operator++()
        {
            this->advance(1);
            return (*this);
        }

        Iterator<containerT, AdvanceDistanceFunctorT, BeginEndFunctorT> operator++(int)
        {
            auto TempIter(*this);
            this->advance(1);
            return TempIter;
        }

        Iterator<containerT, AdvanceDistanceFunctorT, BeginEndFunctorT>& operator--()
        {
            this->advance(-1);
            return (*this);
        }

        Iterator<containerT, AdvanceDistanceFunctorT, BeginEndFunctorT> operator--(int)
        {
            auto TempIter(*this);
            this->advance(-1);
            return TempIter;
        }

        Iterator<containerT, AdvanceDistanceFunctorT, BeginEndFunctorT>& operator+=(const ptrdiff_t& Offset)
        {
            this->advance(Offset);
            return (*this);
        }

        Iterator<containerT, AdvanceDistanceFunctorT, BeginEndFunctorT>& operator-=(const ptrdiff_t& Offset)
        {
            this->advance(-Offset);
            return (*this);
        }

        Iterator<containerT, AdvanceDistanceFunctorT, BeginEndFunctorT> operator+(const ptrdiff_t& Offset) const
        {
            auto NewIter = (*this);
            NewIter.advance(Offset);
            return NewIter;
        }

        Iterator<containerT, AdvanceDistanceFunctorT, BeginEndFunctorT> operator-(const ptrdiff_t& Offset) const
        {
            auto NewIter = (*this);
            NewIter.advance(-Offset);
            return NewIter;
        }

        ptrdiff_t operator-(const Iterator<containerT,AdvanceDistanceFunctorT,BeginEndFunctorT>& iterator) const
        {
            return AdvanceDistanceFunctorT::template distance<Iterator<containerT,AdvanceDistanceFunctorT,BeginEndFunctorT>>(iterator.getDistanceFromBeginToIter(), this->getDistanceFromBeginToIter(), this->getDistanceFromIterToEnd(), iterator.getDistanceFromIterToEnd());
        }

        void setContainerPtr(containerT& container)
        {
            auto containerPtr = get_shared_ptr(container);
            if(containerPtr != m_containerPtr)
            {
                m_containerPtr = containerPtr;
                if(m_containerPtr)
                {
                    (*this) = this->begin();
                }
            }
        }

        void setContainerPtr(const containerT& container)
        {
            auto containerPtr = &container;
            if(containerPtr != m_containerPtr)
            {
                m_containerPtr = containerPtr;
                if(m_containerPtr)
                {
                    (*this) = this->begin();
                }
            }
        }

        void setContainerPtr(containerT* rawContainerPtr)
        {
            auto containerPtr = rawContainerPtr;
            if(containerPtr != m_containerPtr)
            {
                m_containerPtr = containerPtr;
                if(m_containerPtr)
                {
                    (*this) = this->begin();
                }
            }
        }

        const iterator& getPtr()const{return m_ptr;}
        const containerPtrT& getContainerPtr()const{return m_containerPtr;}

        ptrdiff_t getDistanceFromBeginToIter()const
        {
            if(m_containerPtr)
                return std::distance(BeginEndFunctorT::begin(*m_containerPtr),this->m_ptr);
            else
                return 0;
        }

        ptrdiff_t getDistanceFromIterToEnd()const
        {
            if(m_containerPtr)
                return std::distance(this->m_ptr,BeginEndFunctorT::end(*m_containerPtr));
            else
                return 0;
        }

        Iterator<containerT, AdvanceDistanceFunctorT, BeginEndFunctorT> begin() const
        {
            if(m_containerPtr)
            {
                return Iterator<containerT, AdvanceDistanceFunctorT, BeginEndFunctorT>(BeginEndFunctorT::begin(*m_containerPtr), m_containerPtr);
            }
            else
            {
                return Iterator<containerT, AdvanceDistanceFunctorT, BeginEndFunctorT>(m_containerPtr);
            }
        }

        Iterator<containerT, AdvanceDistanceFunctorT, BeginEndFunctorT> end()const
        {
            if(m_containerPtr)
            {
                return Iterator<containerT, AdvanceDistanceFunctorT, BeginEndFunctorT>(BeginEndFunctorT::end(*m_containerPtr), m_containerPtr);
            }
            else
            {
                return Iterator<containerT, AdvanceDistanceFunctorT, BeginEndFunctorT>(m_containerPtr);
            }
        }

        dataTRef operator[](const size_t& index){return (*(this->begin() + index));}
        const dataTRef operator[](const size_t& index)const{return (*(this->begin() + index));}

        size_t size()const
        {
            if(m_containerPtr)
                return m_containerPtr->size();
            else
                return 0;
        }

        size_t length()const{return this->size();}
        size_t max_size()const{return this->size();}
        bool empty()const{return (this->size() == 0);}

    private:
        
        void advance(const ptrdiff_t& HowManyStepsToAdvanceIter)
        {
            if(m_containerPtr)
            {
                AdvanceDistanceFunctorT::advance(m_ptr, HowManyStepsToAdvanceIter, this->begin().m_ptr, this->end().m_ptr, this->getDistanceFromBeginToIter(), this->getDistanceFromIterToEnd());
            }
        }
    };

}

#endif//QOR_PP_H_ITERATORS_SMART
