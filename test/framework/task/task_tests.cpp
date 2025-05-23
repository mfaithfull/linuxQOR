// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "src/configuration/configuration.h"

#include <ostream>
#include <string>
#include <type_traits>

#include "src/qor/test/test.h"
#include "src/qor/assert/assert.h"
#include "src/framework/task/task.h"
#include "src/qor/sync/singleconsumerevent.h"
#include "src/framework/task/syncwait.h"
#include "src/framework/task/whenallready.h"
#include "src/framework/task/fmap.h"
#include "counted.h"

using namespace qor;
using namespace qor::test;
//using namespace qor::framework;

struct TaskTestSuite{};

qor_pp_test_suite_case(TaskTestSuite, task_doesn_t_start_until_awaited)
{
	bool started = false;
	auto func = [&]() -> task<>
	{
		started = true;
		co_return;
	};

	sync_wait([&]() -> task<>
	{
		auto t = func();
		qor_pp_assert_that(!started);

		co_await t;

		qor_pp_assert_that(started);
	}());
}

qor_pp_test_suite_case(TaskTestSuite, awaiting_default_constructed_task_throws_broken_promise)
{
	sync_wait([&]() -> task<>
	{
		task<> t;
		qor_pp_test_assert_throw(co_await t, broken_promise);
	}());
}

qor_pp_test_suite_case(TaskTestSuite, awaiting_task_that_completes_asynchronously)
{
	bool reachedBeforeEvent = false;
	bool reachedAfterEvent = false;
	SingleConsumerEvent event;
	auto f = [&]() -> task<>
	{
		reachedBeforeEvent = true;
		co_await event;
		reachedAfterEvent = true;
	};

	sync_wait([&]() -> task<>
	{
		auto t = f();

		qor_pp_assert_that(!reachedBeforeEvent);

		(void)co_await when_all_ready(
			[&]() -> task<>
			{
				co_await t;
				qor_pp_assert_that(reachedBeforeEvent);
				qor_pp_assert_that(reachedAfterEvent);
			}(),
			[&]() -> task<>
			{
				qor_pp_assert_that(reachedBeforeEvent);
				qor_pp_assert_that(!reachedAfterEvent);
				event.set();
				qor_pp_assert_that(reachedAfterEvent);
				co_return;
			}());
	}());
}

qor_pp_test_suite_case(TaskTestSuite, destroying_task_that_was_never_awaited_destroys_captured_args)
{
	counted::reset_counts();

	auto f = [](counted c) -> task<counted>
	{
		co_return c;
	};

	qor_pp_assert_that(counted::active_count() == 0);

	{
		auto t = f(counted{});
		qor_pp_assert_that(counted::active_count() == 1);
	}

	qor_pp_assert_that(counted::active_count() == 0);
}

qor_pp_test_suite_case(TaskTestSuite, task_destructor_destroys_result)
{
	counted::reset_counts();

	auto f = []() -> task<counted>
	{
		co_return counted{};
	};

	{
		auto t = f();
		qor_pp_assert_that(counted::active_count() == 0);

		auto& result = sync_wait(t);

		qor_pp_assert_that(counted::active_count() == 1);
		qor_pp_assert_that(result.id == 0);
	}

	qor_pp_assert_that(counted::active_count() == 0);
}

qor_pp_test_suite_case(TaskTestSuite, task_of_reference_type)
{
	int value = 3;
	auto f = [&]() -> task<int&>
	{
		co_return value;
	};

	sync_wait([&]() -> task<>
	{
		//SUBCASE("awaiting rvalue task")
		{
			decltype(auto) result = co_await f();
			static_assert(
				std::is_same<decltype(result), int&>::value,
				"co_await r-value reference of task<int&> should result in an int&");
			qor_pp_assert_that(&result == &value);
		}

		//SUBCASE("awaiting lvalue task")
		{
			auto t = f();
			decltype(auto) result = co_await t;
			static_assert(
				std::is_same<decltype(result), int&>::value,
				"co_await l-value reference of task<int&> should result in an int&");
			qor_pp_assert_that(&result == &value);
		}
	}());
}

qor_pp_test_suite_case(TaskTestSuite, passing_parameter_by_value_to_task_coroutine_calls_move_constructor_exactly_once)
{
	counted::reset_counts();

	auto f = [](counted arg) -> task<>
	{
		co_return;
	};

	counted c;

	qor_pp_assert_that(counted::active_count() == 1);
	qor_pp_assert_that(counted::default_construction_count == 1);
	qor_pp_assert_that(counted::copy_construction_count == 0);
	qor_pp_assert_that(counted::move_construction_count == 0);
	qor_pp_assert_that(counted::destruction_count == 0);

	{
		auto t = f(c);

		// Should have called copy-constructor to pass a copy of 'c' into f by value.
		qor_pp_assert_that(counted::copy_construction_count == 1);

		// Inside f it should have move-constructed parameter into coroutine frame variable
		//WARN_MESSAGE(counted::move_construction_count == 1,
		//	"Known bug in MSVC 2017.1, not critical if it performs multiple moves");

		// Active counts should be the instance 'c' and the instance captured in coroutine frame of 't'.
		qor_pp_assert_that(counted::active_count() == 2);
	}

	qor_pp_assert_that(counted::active_count() == 1);
}

qor_pp_test_suite_case(TaskTestSuite, task_void_fmap_pipe_operator)
{
	//using fmap;

	SingleConsumerEvent event;

	auto f = [&]() -> task<>
	{
		co_await event;
		co_return;
	};

	auto t = f() | fmap([] { return 123; });

	sync_wait(when_all_ready(
		[&]() -> task<>
		{
			qor_pp_assert_that(co_await t == 123);
		}(),
		[&]() -> task<>
		{
			event.set();
			co_return;
		}()));
}

qor_pp_test_suite_case(TaskTestSuite, task_int_fmap_pipe_operator)
{
	//using task;
	//using fmap;
	//using sync_wait;
	//using make_task;

	auto one = [&]() -> task<int>
	{
		co_return 1;
	};

	//("r-value fmap / r-value lambda")
	{
		auto t = one()
			| fmap([delta = 1](auto i) { return i + delta; });
		qor_pp_assert_that(sync_wait(t) == 2);
	}

	//("r-value fmap / l-value lambda")
	{
		using namespace std::string_literals;

		auto t = [&]
		{
			auto f = [prefix = "pfx"s](int x)
			{
				return prefix + std::to_string(x);
			};

			// Want to make sure that the resulting awaitable has taken
			// a copy of the lambda passed to fmap().
			return one() | fmap(f);
		}();

		qor_pp_assert_that(sync_wait(t) == "pfx1");
	}

	//("l-value fmap / r-value lambda")
	{
		using namespace std::string_literals;

		auto t = [&]
		{
			auto addprefix = fmap([prefix = "a really really long prefix that prevents small string optimisation"s](int x)
			{
				return prefix + std::to_string(x);
			});

			// Want to make sure that the resulting awaitable has taken
			// a copy of the lambda passed to fmap().
			return one() | addprefix;
		}();

		qor_pp_assert_that(sync_wait(t) == "a really really long prefix that prevents small string optimisation1");
	}

	//("l-value fmap / l-value lambda")
	{
		using namespace std::string_literals;

		task<std::string> t;

		{
			auto lambda = [prefix = "a really really long prefix that prevents small string optimisation"s](int x)
			{
				return prefix + std::to_string(x);
			};

			auto addprefix = fmap(lambda);

			// Want to make sure that the resulting task has taken
			// a copy of the lambda passed to fmap().
			t = make_task(one() | addprefix);
		}

		qor_pp_assert_that(!t.is_ready());

		qor_pp_assert_that(sync_wait(t) == "a really really long prefix that prevents small string optimisation1");
	}
}

qor_pp_test_suite_case(TaskTestSuite, chained_fmap_pipe_operations)
{
	using namespace std::string_literals;
	//using task;
	//using sync_wait;

	auto prepend = [](std::string s)
	{
		//using fmap;
		return fmap([s = std::move(s)](const std::string& value) { return s + value; });
	};

	auto append = [](std::string s)
	{
		//using fmap;
		return fmap([s = std::move(s)](const std::string& value){ return value + s; });
	};

	auto asyncString = [](std::string s) -> task<std::string>
	{
		co_return std::move(s);
	};

	auto t = asyncString("base"s) | prepend("pre_"s) | append("_post"s);

	qor_pp_assert_that(sync_wait(t) == "pre_base_post");
}

qor_pp_test_suite_case(TaskTestSuite, lots_of_synchronous_completions_doesnt_result_in_stack_overflow)
{
	auto completesSynchronously = []() -> task<int>
	{
		co_return 1;
	};

	auto run = [&]() -> task<>
	{
		int sum = 0;
		for (int i = 0; i < 1'000'000; ++i)
		{
			sum += co_await completesSynchronously();
		}
		qor_pp_assert_that(sum == 1'000'000);
	};

	sync_wait(run());
}

