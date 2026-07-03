// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license
#pragma once

#include <memory>

#include "../assert.h"
#include "base.h"
#include "../util/failmessage.h"

namespace qor{ namespace assert{ namespace subject{

	template <typename T>
	class WeakPtrSubject 
	{
		const char* d_file;
		const int d_line;
		const T d_value;
		const bool d_failOnError;

		void fail(const std::string msg) 
		{
			//TODO: FAIL() << "Expected: " << msg << "\nGot: "<< "blah blah";
		}

		public:

		WeakPtrSubject(const bool failOnError, const char* file, int line, const T& v) : d_file(file), d_line(line), d_value(v), d_failOnError(failOnError) {}

		WeakPtrSubject<T>& isExpired() {
			if (!d_value.expired()) 
			{
				util::FailMessage::create()
				.failOnError(d_failOnError)
				.file(d_file)
				.line(d_line)
				.fact("Expected the weak_ptr to be expired")
				.fact("Got", d_value);
			}
			return *this;
		}

		WeakPtrSubject<T>& isNotExpired() 
		{
			if (d_value.expired()) 
			{
				util::FailMessage::create()
				.failOnError(d_failOnError)
				.file(d_file)
				.line(d_line)
				.fact("Expected the weak_ptr to not be expired")
				.fact("Got", d_value);
			}
			return *this;
		}

		auto value() 
		{
			auto sp = d_value.lock();
			return assert_that_internal(qor::assert::Adl(), d_failOnError, d_file, d_line, *sp);
		}
	};

}}}//qor::assert::subject
