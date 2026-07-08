// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_GENERATOR
#define QOR_PP_H_FRAMEWORK_TASK_GENERATOR

#include <coroutine>
#include <type_traits>
#include <utility>
#include <exception>
#include <iterator>
#include <functional>

namespace qor{

	template<typename T> class Generator;

	namespace detail{

		template<typename T>
		class GeneratorPromise
		{
		public:

			using value_type = std::remove_reference_t<T>;
			using reference_type = std::conditional_t<std::is_reference_v<T>, T, T&>;
			using pointer_type = value_type*;

			GeneratorPromise() = default;

			Generator<T> get_return_object() noexcept;

			constexpr std::suspend_always initial_suspend() const noexcept 
			{ 
				return {}; 
			}

			constexpr std::suspend_always final_suspend() const noexcept 
			{ 
				return {}; 
			}

			template< typename U = T, std::enable_if_t<!std::is_rvalue_reference<U>::value, int> = 0>
			std::suspend_always yield_value(std::remove_reference_t<T>& value) noexcept
			{
				m_value = std::addressof(value);
				return {};
			}

			std::suspend_always yield_value(std::remove_reference_t<T>&& value) noexcept
			{
				m_value = std::addressof(value);
				return {};
			}

			void unhandled_exception()
			{
				m_exception = std::current_exception();
			}

			void return_void()
			{
			}

			reference_type value() const noexcept
			{
				return static_cast<reference_type>(*m_value);
			}

			// Don't allow any use of 'co_await' inside the Generator coroutine.
			template<typename U>
			std::suspend_never await_transform(U&& value) = delete;

			void rethrow_if_exception()
			{
				if (m_exception)
				{
					std::rethrow_exception(m_exception);
				}
			}

		private:

			pointer_type m_value{ nullptr };
			std::exception_ptr m_exception{};

		};

        struct generator_sentinel {};

		template<typename T>
		class generator_iterator
		{
			using coroutine_handle = std::coroutine_handle<GeneratorPromise<T>>;

		public:

			using iterator_category = std::input_iterator_tag;
			// What type should we use for counting elements of a potentially infinite sequence?
			using difference_type = std::ptrdiff_t;
			using value_type = typename GeneratorPromise<T>::value_type;
			using reference = typename GeneratorPromise<T>::reference_type;
			using pointer = typename GeneratorPromise<T>::pointer_type;

			// Iterator needs to be default-constructible to satisfy the Range concept.
			generator_iterator() noexcept : m_coroutine(nullptr) {}
			
			explicit generator_iterator(coroutine_handle coroutine) noexcept : m_coroutine(coroutine) {}

			friend bool operator==(const generator_iterator& it, generator_sentinel) noexcept
			{
				return !it.m_coroutine || it.m_coroutine.done();
			}

			friend bool operator!=(const generator_iterator& it, generator_sentinel s) noexcept
			{
				return !(it == s);
			}

			friend bool operator==(generator_sentinel s, const generator_iterator& it) noexcept
			{
				return (it == s);
			}

			friend bool operator!=(generator_sentinel s, const generator_iterator& it) noexcept
			{
				return it != s;
			}

			generator_iterator& operator++()
			{
				m_coroutine.resume();
				if (m_coroutine.done())
				{
					m_coroutine.promise().rethrow_if_exception();
				}

				return *this;
			}

			// Need to provide post-increment operator to implement the 'Range' concept.
			void operator++(int)
			{
				(void)operator++();
			}

			reference operator*() const noexcept
			{
				return m_coroutine.promise().value();
			}

			pointer operator->() const noexcept
			{
				return std::addressof(operator*());
			}

		private:

			coroutine_handle m_coroutine;
		};
	}

	template<typename T>
	class [[nodiscard]] Generator
	{
	public:

		using promise_type = detail::GeneratorPromise<T>;
		using iterator = detail::generator_iterator<T>;

		Generator() noexcept : m_coroutine(nullptr)	{}

		Generator(Generator&& other) noexcept : m_coroutine(other.m_coroutine)
		{
			other.m_coroutine = nullptr;
		}

		Generator(const Generator& other) = delete;

		~Generator()
		{
			if (m_coroutine)
			{
				m_coroutine.destroy();
			}
		}

		Generator& operator=(Generator other) noexcept
		{
			swap(other);
			return *this;
		}

		iterator begin()
		{
			if (m_coroutine)
			{
				m_coroutine.resume();
				if (m_coroutine.done())
				{
					m_coroutine.promise().rethrow_if_exception();
				}
			}

			return iterator{ m_coroutine };
		}

		detail::generator_sentinel end() noexcept
		{
			return detail::generator_sentinel{};
		}

		void swap(Generator& other) noexcept
		{
			std::swap(m_coroutine, other.m_coroutine);
		}

	private:

		friend class detail::GeneratorPromise<T>;

		explicit Generator(std::coroutine_handle<promise_type> coroutine) noexcept : m_coroutine(coroutine) {}

		std::coroutine_handle<promise_type> m_coroutine;

	};

	template<typename T>
	void swap(Generator<T>& a, Generator<T>& b)
	{
		a.swap(b);
	}

	namespace detail
	{
		template<typename T>
		Generator<T> GeneratorPromise<T>::get_return_object() noexcept
		{
			using coroutine_handle = std::coroutine_handle<GeneratorPromise<T>>;
			return Generator<T>{ coroutine_handle::from_promise(*this) };
		}
	}

	template<typename FUNC, typename T>
	Generator<std::invoke_result_t<FUNC&, typename Generator<T>::iterator::reference>> fmap(FUNC func, Generator<T> source)
	{
		for (auto&& value : source)
		{
			co_yield std::invoke(func, static_cast<decltype(value)>(value));
		}
	}

}//qor

#endif//QOR_PP_H_FRAMEWORK_TASK_GENERATOR
