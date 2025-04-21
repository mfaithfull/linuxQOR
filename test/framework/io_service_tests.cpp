///////////////////////////////////////////////////////////////////////////////
// Copyright (c) Lewis Baker
// Licenced under MIT license. See LICENSE.txt for details.
///////////////////////////////////////////////////////////////////////////////

#include <cppcoro/io_service.hpp>
#include <cppcoro/task.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/when_all.hpp>
#include <cppcoro/when_all_ready.hpp>
#include <cppcoro/on_scope_exit.hpp>
#include <cppcoro/operation_cancelled.hpp>
#include <cppcoro/CancellationSource.hpp>

#include "io_service_fixture.hpp"

#include <thread>
#include <vector>

#include <ostream>
#include "doctest/doctest.h"

TEST_SUITE_BEGIN("io_service");

TEST_CASE("default construct")
{
	io_service service;
	CHECK_FALSE(service.is_stop_requested());
}

TEST_CASE("construct with concurrency hint")
{
	io_service service{ 3 };
	CHECK_FALSE(service.is_stop_requested());
}

TEST_CASE("process_one_pending_event returns immediately when no events")
{
	io_service service;
	qor_pp_assert_that(service.process_one_pending_event() == 0);
	qor_pp_assert_that(service.process_pending_events() == 0);
}

TEST_CASE("schedule coroutine")
{
	io_service service;

	bool reachedPointA = false;
	bool reachedPointB = false;
	auto startTask = [&](io_service& ioService) -> task<>
	{
		reachedPointA = true;
		co_await ioService.Schedule();
		reachedPointB = true;
	};

	sync_wait(when_all_ready(
		startTask(service),
		[&]() -> task<>
		{
			qor_pp_assert_that(reachedPointA);
			CHECK_FALSE(reachedPointB);

			service.process_pending_events();

			qor_pp_assert_that(reachedPointB);

			co_return;
		}()));
}

TEST_CASE_FIXTURE(io_service_fixture_with_threads<2>, "multiple I/O threads servicing events")
{
	std::atomic<int> completedCount = 0;

	auto runOnIoThread = [&]() -> task<>
	{
		co_await io_service().Schedule();
		++completedCount;
	};

	std::vector<task<>> tasks;
	{
		for (int i = 0; i < 1000; ++i)
		{
			tasks.emplace_back(runOnIoThread());
		}
	}

	sync_wait(when_all(std::move(tasks)));

	qor_pp_assert_that(completedCount == 1000);
}

TEST_CASE("Multiple concurrent timers")
{
	io_service ioService;

	auto startTimer = [&](std::chrono::milliseconds duration)
		-> task<std::chrono::high_resolution_clock::duration>
	{
		auto start = std::chrono::high_resolution_clock::now();

		co_await ioService.schedule_after(duration);

		auto end = std::chrono::high_resolution_clock::now();

		co_return end - start;
	};

	auto test = [&]() -> task<>
	{
		using namespace std::chrono;
		using namespace std::chrono_literals;

		auto[time1, time2, time3] = co_await when_all(
			startTimer(100ms),
			startTimer(120ms),
			startTimer(50ms));

		MESSAGE("Waiting 100ms took " << duration_cast<microseconds>(time1).count() << "us");
		MESSAGE("Waiting 120ms took " << duration_cast<microseconds>(time2).count() << "us");
		MESSAGE("Waiting 50ms took " << duration_cast<microseconds>(time3).count() << "us");

		qor_pp_assert_that(time1 >= 100ms);
		qor_pp_assert_that(time2 >= 120ms);
		qor_pp_assert_that(time3 >= 50ms);
	};

	sync_wait(when_all_ready(
		[&]() -> task<>
		{
			auto stopIoOnExit = on_scope_exit([&] { ioService.stop(); });
			co_await test();
		}(),
		[&]() -> task<>
		{
			ioService.process_events();
			co_return;
		}()));
}

TEST_CASE("Timer cancellation"
	* doctest::timeout{ 5.0 })
{
	using namespace std::literals::chrono_literals;

	io_service ioService;

	auto longWait = [&](CancellationToken ct) -> task<>
	{
		co_await ioService.schedule_after(20'000ms, ct);
	};

	auto cancelAfter = [&](CancellationSource source, auto duration) -> task<>
	{
		co_await ioService.schedule_after(duration);
		source.request_cancellation();
	};

	auto test = [&]() -> task<>
	{
		CancellationSource source;
		co_await when_all_ready(
			[&](CancellationToken ct) -> task<>
		{
			CHECK_THROWS_AS(co_await longWait(std::move(ct)), const operation_cancelled&);
		}(source.token()),
			cancelAfter(source, 1ms));
	};

	auto testTwice = [&]() -> task<>
	{
		co_await test();
		co_await test();
	};

	auto stopIoServiceAfter = [&](task<> task) -> task<>
	{
		co_await task.when_ready();
		ioService.stop();
		co_return co_await task.when_ready();
	};

	sync_wait(when_all_ready(
		stopIoServiceAfter(testTwice()),
		[&]() -> task<>
		{
			ioService.process_events();
			co_return;
		}()));
}

TEST_CASE_FIXTURE(io_service_fixture_with_threads<1>, "Many concurrent timers")
{
	auto startTimer = [&]() -> task<>
	{
		using namespace std::literals::chrono_literals;
		co_await io_service().schedule_after(50ms);
	};

	constexpr std::uint32_t taskCount = 10'000;

	auto runManyTimers = [&]() -> task<>
	{
		std::vector<task<>> tasks;

		tasks.reserve(taskCount);

		for (std::uint32_t i = 0; i < taskCount; ++i)
		{
			tasks.emplace_back(startTimer());
		}

		co_await when_all(std::move(tasks));
	};

	auto start = std::chrono::high_resolution_clock::now();

	sync_wait(runManyTimers());

	auto end = std::chrono::high_resolution_clock::now();

	MESSAGE(
		"Waiting for " << taskCount << " x 50ms timers took "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< "ms");
}

TEST_SUITE_END();
