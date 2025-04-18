///////////////////////////////////////////////////////////////////////////////
// Copyright (c) Lewis Baker
// Licenced under MIT license. See LICENSE.txt for details.
///////////////////////////////////////////////////////////////////////////////

#include <cppcoro/schedule_on.hpp>
#include <cppcoro/resume_on.hpp>
#include <cppcoro/io_service.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/when_all_ready.hpp>
#include <cppcoro/on_scope_exit.hpp>
#include <cppcoro/fmap.hpp>

#include "io_service_fixture.hpp"

#include <ostream>
#include "doctest/doctest.h"

TEST_SUITE_BEGIN("schedule/resume_on");

TEST_CASE_FIXTURE(io_service_fixture, "schedule_on task<> function")
{
	auto mainThreadId = std::this_thread::get_id();

	std::thread::id ioThreadId;

	auto start = [&]() -> task<>
	{
		ioThreadId = std::this_thread::get_id();
		qor_pp_assert_that(ioThreadId != mainThreadId);
		co_return;
	};

	sync_wait([&]() -> task<>
	{
		qor_pp_assert_that(std::this_thread::get_id() == mainThreadId);

		co_await schedule_on(io_service(), start());

		// TODO: Uncomment this check once the implementation of task<T>
		// guarantees that the continuation will resume on the same thread
		// that the task completed on. Currently it's possible to resume on
		// the thread that launched the task if it completes on another thread
		// before the current thread could attach the continuation after it
		// suspended. See cppcoro issue #79.
		//
		// The long-term solution here is to use the symmetric-transfer capability
		// to avoid the use of atomics and races, but we're still waiting for MSVC to
		// implement this (doesn't seem to be implemented as of VS 2017.8 Preview 5)
		//qor_pp_assert_that(std::this_thread::get_id() == ioThreadId);
	}());
}

TEST_CASE_FIXTURE(io_service_fixture, "schedule_on async_generator<> function")
{
	auto mainThreadId = std::this_thread::get_id();

	std::thread::id ioThreadId;

	auto makeSequence = [&]() -> async_generator<int>
	{
		ioThreadId = std::this_thread::get_id();
		qor_pp_assert_that(ioThreadId != mainThreadId);

		co_yield 1;

		qor_pp_assert_that(std::this_thread::get_id() == ioThreadId);

		co_yield 2;

		qor_pp_assert_that(std::this_thread::get_id() == ioThreadId);

		co_yield 3;

		qor_pp_assert_that(std::this_thread::get_id() == ioThreadId);

		co_return;
	};

	io_service otherIoService;

	sync_wait(when_all_ready(
		[&]() -> task<>
	{
		qor_pp_assert_that(std::this_thread::get_id() == mainThreadId);

		auto seq = schedule_on(io_service(), makeSequence());

		int expected = 1;
		for (auto iter = co_await seq.begin(); iter != seq.end(); co_await ++iter)
		{
			int value = *iter;
			qor_pp_assert_that(value == expected++);

			// Transfer exection back to main thread before
			// awaiting next item in the loop to chck that
			// the generator is resumed on io_service() thread.
			co_await otherIoService.schedule();
		}

		otherIoService.stop();
	}(),
		[&]() -> task<>
	{
		otherIoService.process_events();
		co_return;
	}()));
}

TEST_CASE_FIXTURE(io_service_fixture, "resume_on task<> function")
{
	auto mainThreadId = std::this_thread::get_id();

	auto start = [&]() -> task<>
	{
		qor_pp_assert_that(std::this_thread::get_id() == mainThreadId);
		co_return;
	};

	sync_wait([&]() -> task<>
	{
		qor_pp_assert_that(std::this_thread::get_id() == mainThreadId);

		co_await resume_on(io_service(), start());

		// NOTE: This check could potentially spuriously fail with the current
		// implementation of task<T>. See cppcoro issue #79.
		qor_pp_assert_that(std::this_thread::get_id() != mainThreadId);
	}());
}

constexpr bool isMsvc15_4X86Optimised =
#if defined(_MSC_VER) && _MSC_VER == 1911 && defined(_M_IX86) && !defined(_DEBUG)
	true;
#else
	false;
#endif

// Disable under MSVC 15.4 X86 Optimised due to presumed compiler bug that causes
// an access violation. Seems to be fixed under MSVC 15.5.
TEST_CASE_FIXTURE(io_service_fixture, "resume_on async_generator<> function"
	* doctest::skip{ isMsvc15_4X86Optimised })
{
	auto mainThreadId = std::this_thread::get_id();

	std::thread::id ioThreadId;

	auto makeSequence = [&]() -> async_generator<int>
	{
		co_await io_service().schedule();

		ioThreadId = std::this_thread::get_id();

		qor_pp_assert_that(ioThreadId != mainThreadId);

		co_yield 1;

		co_yield 2;

		co_await io_service().schedule();

		co_yield 3;

		co_await io_service().schedule();

		co_return;
	};

	io_service otherIoService;

	sync_wait(when_all_ready(
		[&]() -> task<>
	{
		auto stopOnExit = on_scope_exit([&] { otherIoService.stop(); });

		qor_pp_assert_that(std::this_thread::get_id() == mainThreadId);

		auto seq = resume_on(otherIoService, makeSequence());

		int expected = 1;
		for (auto iter = co_await seq.begin(); iter != seq.end(); co_await ++iter)
		{
			int value = *iter;
			// Every time we receive a value it should be on our requested
			// scheduler (ie. main thread)
			qor_pp_assert_that(std::this_thread::get_id() == mainThreadId);
			qor_pp_assert_that(value == expected++);

			// Occasionally transfer execution to a different thread before
			// awaiting next element.
			if (value == 2)
			{
				co_await io_service().schedule();
			}
		}

		otherIoService.stop();
	}(),
		[&]() -> task<>
	{
		otherIoService.process_events();
		co_return;
	}()));
}

TEST_CASE_FIXTURE(io_service_fixture, "schedule_on task<> pipe syntax")
{
	auto mainThreadId = std::this_thread::get_id();

	auto makeTask = [&]() -> task<int>
	{
		qor_pp_assert_that(std::this_thread::get_id() != mainThreadId);
		co_return 123;
	};

	auto triple = [&](int x)
	{
		qor_pp_assert_that(std::this_thread::get_id() != mainThreadId);
		return x * 3;
	};

	qor_pp_assert_that(sync_wait(makeTask() | schedule_on(io_service())) == 123);

	// Shouldn't matter where in sequence schedule_on() appears since it applies
	// at the start of the pipeline (ie. before first task starts).
	qor_pp_assert_that(sync_wait(makeTask() | schedule_on(io_service()) | fmap(triple)) == 369);
	qor_pp_assert_that(sync_wait(makeTask() | fmap(triple) | schedule_on(io_service())) == 369);
}

TEST_CASE_FIXTURE(io_service_fixture, "resume_on task<> pipe syntax")
{
	auto mainThreadId = std::this_thread::get_id();

	auto makeTask = [&]() -> task<int>
	{
		qor_pp_assert_that(std::this_thread::get_id() == mainThreadId);
		co_return 123;
	};

	sync_wait([&]() -> task<>
	{
		task<int> t = makeTask() | resume_on(io_service());
		qor_pp_assert_that(co_await t == 123);
		qor_pp_assert_that(std::this_thread::get_id() != mainThreadId);
	}());
}

TEST_CASE_FIXTURE(io_service_fixture, "resume_on task<> pipe syntax multiple uses")
{
	auto mainThreadId = std::this_thread::get_id();

	auto makeTask = [&]() -> task<int>
	{
		qor_pp_assert_that(std::this_thread::get_id() == mainThreadId);
		co_return 123;
	};

	auto triple = [&](int x)
	{
		qor_pp_assert_that(std::this_thread::get_id() != mainThreadId);
		return x * 3;
	};

	io_service otherIoService;

	sync_wait(when_all_ready(
		[&]() -> task<>
	{
		auto stopOnExit = on_scope_exit([&] { otherIoService.stop(); });

		qor_pp_assert_that(std::this_thread::get_id() == mainThreadId);

		task<int> t =
			makeTask()
			| resume_on(io_service())
			| fmap(triple)
			| resume_on(otherIoService);

		qor_pp_assert_that(co_await t == 369);

		qor_pp_assert_that(std::this_thread::get_id() == mainThreadId);
	}(),
		[&]() -> task<>
	{
		otherIoService.process_events();
		co_return;
	}()));
}

TEST_SUITE_END();
