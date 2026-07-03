// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ITERATORS_ITERATORS
#define QOR_PP_H_ITERATORS_ITERATORS

#include "rawiterators.h"
#include "iteratorfunctors.h"
#include "iterator.h"

namespace qor{

    // Define some useful linear iterators

    template<typename containerT>
    class LinearIterator : public Iterator<containerT,AdvanceLinearly,BeginEnd>{using Iterator<containerT,AdvanceLinearly,BeginEnd>::Iterator;};

    template<typename containerT>
    class LinearConstIterator : public Iterator<containerT,AdvanceLinearly,cBeginEnd>{using Iterator<containerT,AdvanceLinearly,cBeginEnd>::Iterator;};

    template<typename containerT>
    class LinearReverseIterator : public Iterator<containerT,AdvanceLinearly,rBeginEnd>{using Iterator<containerT,AdvanceLinearly,rBeginEnd>::Iterator;};

    template<typename containerT>
    class LinearConstReverseIterator : public Iterator<containerT,AdvanceLinearly,crBeginEnd>{using Iterator<containerT,AdvanceLinearly,crBeginEnd>::Iterator;};

    // Define some useful circular iterators

    template<typename containerT>
    class CircularIterator : public Iterator<containerT,AdvanceCircularly,BeginEnd>{using Iterator<containerT,AdvanceCircularly,BeginEnd>::Iterator;};

    template<typename containerT>
    class CircularConstIterator : public Iterator<containerT,AdvanceCircularly,cBeginEnd>{using Iterator<containerT,AdvanceCircularly,cBeginEnd>::Iterator;};

    template<typename containerT>
    class CircularReverseIterator : public Iterator<containerT,AdvanceCircularly,rBeginEnd>{using Iterator<containerT,AdvanceCircularly,rBeginEnd>::Iterator;};

    template<typename containerT>
    class CircularConstReverseIterator : public Iterator<containerT,AdvanceCircularly,crBeginEnd>{using Iterator<containerT,AdvanceCircularly,crBeginEnd>::Iterator;};

}//qor

#endif//QOR_PP_H_ITERATORS_ITERATORS
