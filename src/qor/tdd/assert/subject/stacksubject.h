// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license


#ifndef QOR_PP_H_SRC_QOR_TDD_ASSERT_SUBJECT_STACKSUBJECT
#define QOR_PP_H_SRC_QOR_TDD_ASSERT_SUBJECT_STACKSUBJECT

#include <stack>

#include "../proposition/hassizepropositions.h"
#include "../proposition/isemptypropositions.h"
#include "../proposition/isequaltopropositions.h"
#include "base.h"

namespace qor{ namespace assert{ namespace subject{

      template <typename T>
      class StackSubject : public virtual Base,
                        public proposition::HasSizePropositions<StackSubject<T>, T>,
                        public proposition::IsEmptyPropositions<StackSubject<T>, T>,
                        public proposition::IsEqualToPropositions<StackSubject<T>, T> 
      {
            const T d_value;

      protected:
            const T* getValue() const override { return &d_value; }

      public:
            StackSubject(const bool failOnError, const char* file, int line, const T& v)
            : Base(failOnError, file, line), d_value(v) {}
      };

}}}//qor::assert::subject

#endif//QOR_PP_H_SRC_QOR_TDD_ASSERT_SUBJECT_STACKSUBJECT
