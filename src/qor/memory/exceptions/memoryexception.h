// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MEMORY_EXCEPTION
#define QOR_PP_H_MEMORY_EXCEPTION

#include <stdexcept>
#include <format>

namespace qor{

	class memoryexception : public std::logic_error
	{
	public:
		using base_type = std::logic_error;

		template <typename... Args>
		memoryexception(const std::string& fmt, Args&&... args) : base_type(std::vformat(fmt, std::make_format_args(args...))){ }

		explicit memoryexception(const char* message) : base_type(message){ }
	};

}//qor

#endif//QOR_PP_H_MEMORY_EXCEPTION
