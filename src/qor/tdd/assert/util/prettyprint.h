// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#if __cpp_lib_span >= 202002L
#include <span>
#endif
#include <unordered_map>
#include <unordered_set>

namespace qor{ namespace assert{ namespace util {

    template <typename Key, typename T, typename Compare, typename Allocator>
    std::ostream& operator<<(std::ostream& os, const std::map<Key, T, Compare, Allocator>& m) 
    {
		os << "[std::map ";
		if (!m.empty())
		for (const auto& entry : m) os << " " << entry.first << "->" << entry.second;
		os << " ]";
		return os;
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    std::ostream& operator<<(std::ostream& os,const std::unordered_map<Key, T, Hash, KeyEqual, Allocator>& m) 
	{
		os << "[std::unordered_map ";
		if (!m.empty())
		for (const auto& entry : m) os << " " << entry.first << "->" << entry.second;
		os << " ]";
		return os;
    }

    template <typename Key, typename T, typename Compare, typename Allocator>
    std::ostream& operator<<(std::ostream& os, const std::multimap<Key, T, Compare, Allocator>& m) 
	{
		os << "[std::multimap ";
		if (!m.empty())
		for (const auto& entry : m) os << " " << entry.first << "->" << entry.second;
		os << " ]";
		return os;
    }

	template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
	std::ostream& operator<<(std::ostream& os, const std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator>& m) 
	{
		os << "[std::unordered_multimap ";
		if (!m.empty())
		for (const auto& entry : m) os << " " << entry.first << "->" << entry.second;
		os << " ]";
		return os;
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const std::optional<T> opt) 
	{
		os << "[std::optional ";
		if (opt.has_value()) os << opt.value();
		os << " ]";
		return os;
	}

	template <typename T, typename Container, typename Compare>
	std::ostream& operator<<(std::ostream& os, const std::priority_queue<T, Container, Compare>& m) 
	{
		os << "[std::priority_queue "
			<< "size=(" << m.size() << ")";
		if (!m.empty()) 
		{
			os << " top=(" << m.top() << ")";
		}
		os << " ]";
		return os;
	}

#if __cpp_lib_span >= 202002L

	template <typename T, std::size_t Extent>
	std::ostream& operator<<(std::ostream& os, const std::span<T, Extent> data) 
	{
		os << "[std::span ";
		for (const auto& d : data) 
		{
			os << " " << d;
		}
		os << " ]";
		return os;
	}

#endif

	template <typename T, typename C>
	std::ostream& operator<<(std::ostream& os, const std::stack<T, C>& m) 
	{
		os << "[std::stack "
			<< "size=(" << m.size() << ")";
		if (!m.empty()) {
			os << " top=(" << m.top() << ")";
		}
		os << " ]";
		return os;
	}

	template <typename Key, typename Compare, typename Allocator>
	std::ostream& operator<<(std::ostream& os, const std::set<Key, Compare, Allocator>& m) 
	{
		os << "[std::set ";
		if (!m.empty())
		for (const auto& entry : m) os << " " << entry;
		os << " ]";
		return os;
	}

	template <typename Key, typename Compare, typename Allocator>
	std::ostream& operator<<(std::ostream& os, const std::unordered_set<Key, Compare, Allocator>& m) 
	{
		os << "[std::unordered_set ";
		if (!m.empty())
		for (const auto& entry : m) os << " " << entry;
		os << " ]";
		return os;
	}

	template <typename T, typename Deleter>
	std::ostream& operator<<(std::ostream& os, std::unique_ptr<T, Deleter> const& p) 
	{
		os << "[std::unique_ptr ";
		if (p) 
		{
			os << *p;
		}
		os << " ]";
		return os;
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const std::weak_ptr<T> o) 
	{
		os << "[std::weak_ptr ";
		if (!o.expired()) 
		{
			auto sp = o.lock();
			os << sp.get();
		}
		os << " ]";
		return os;
	}

}}}//qor::assert::util
