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

#include <thread>
#include <chrono>
#include <ostream>
#include <coroutine>
#include "src/qor/test/test.h"
#include "src/qor/assert/assert.h"
#include "src/framework/task/multiproducersequencer.h"
#include "src/qor/sync/sequencebarrier.h"
#include "src/qor/sync/sequencetraits.h"
#include "src/qor/sync/onscopeexit.h"
#include "src/framework/task/syncwait.h"
#include "src/framework/task/whenall.h"
#include "src/framework/task/task.h"
#include "src/qor/objectcontext/anyobject.h"
#include "src/framework/thread/currentthread.h"
#include "src/framework/thread/thread.h"
#include "src/framework/thread/threadpool.h"
#include "src/framework/thread/currentprocess.h"

using namespace qor;
using namespace qor::test;
using namespace qor::framework;

struct MultiProducerSequencerTestSuite{};

namespace
{
	task<> one_at_a_time_producer(
		ThreadPool& tp,
		multi_producer_sequencer<std::size_t>& sequencer,
		std::uint64_t buffer[],
		std::uint64_t iterationCount)
	{
		if (iterationCount == 0) co_return;

		co_await tp.Schedule();

		const std::size_t bufferSize = sequencer.buffer_size();
		const std::size_t mask = bufferSize - 1;

		std::uint64_t i = 0;
		while (i < iterationCount)
		{
			auto seq = co_await sequencer.claim_one(tp);
			buffer[seq & mask] = ++i;
			sequencer.publish(seq);
		}

		auto finalSeq = co_await sequencer.claim_one(tp);
		buffer[finalSeq & mask] = 0;
		sequencer.publish(finalSeq);
	}

	task<> batch_producer(
		ThreadPool& tp,
		multi_producer_sequencer<std::size_t>& sequencer,
		std::uint64_t buffer[],
		std::uint64_t iterationCount,
		std::size_t maxBatchSize)
	{
		const std::size_t bufferSize = sequencer.buffer_size();

		std::uint64_t i = 0;
		while (i < iterationCount)
		{
			const std::size_t batchSize = static_cast<std::size_t>(
				std::min<std::uint64_t>(maxBatchSize, iterationCount - i));
			auto sequences = co_await sequencer.claim_up_to(batchSize, tp);
			for (auto seq : sequences)
			{
				buffer[seq % bufferSize] = ++i;
			}
			sequencer.publish(sequences);
		}

		auto finalSeq = co_await sequencer.claim_one(tp);
		buffer[finalSeq % bufferSize] = 0;
		sequencer.publish(finalSeq);
	}

	task<std::uint64_t> consumer(
		ThreadPool& tp,
		const multi_producer_sequencer<std::size_t>& sequencer,
		SequenceBarrier<std::size_t>& readBarrier,
		const std::uint64_t buffer[],
		std::uint32_t producerCount)
	{
		co_await tp.Schedule();

		const std::size_t mask = sequencer.buffer_size() - 1;

		std::uint64_t sum = 0;

		std::uint32_t endCount = 0;
		std::size_t nextToRead = 0;
		do
		{
			std::size_t available = co_await sequencer.wait_until_published(nextToRead, nextToRead - 1, tp);
			do
			{
				const auto& value = buffer[nextToRead & mask];
				sum += value;

				// Zero value is sentinel that indicates the end of one of the streams.
				const bool isEndOfStream = value == 0;
				endCount += isEndOfStream ? 1 : 0;
			} while (nextToRead++ != available);

			// Notify that we've finished processing up to 'available'.
			readBarrier.publish(available);
		} while (endCount < producerCount);

		co_return sum;
	}
}

qor_pp_test_suite_case(MultiProducerSequencerTestSuite, two_producers_batch_single_consumer)
{
	ThreadPool tp{3};
	tp.Setup();
//	static_thread_pool tp{ 3 };

	// Allow time for threads to start up.
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(1ms);

	constexpr std::size_t batchSize = 10;
	constexpr std::size_t bufferSize = 16384;

	SequenceBarrier<std::size_t> readBarrier;
	multi_producer_sequencer<std::size_t> sequencer(readBarrier, bufferSize);

	constexpr std::uint64_t iterationCount = 1'000'000;

	std::uint64_t buffer[bufferSize];

	auto startTime = std::chrono::high_resolution_clock::now();

	constexpr std::uint32_t producerCount = 2;
	auto result = std::get<0>(sync_wait(when_all(
		consumer(tp, sequencer, readBarrier, buffer, producerCount),
		batch_producer(tp, sequencer, buffer, iterationCount, batchSize),
		batch_producer(tp, sequencer, buffer, iterationCount, batchSize))));

	auto endTime = std::chrono::high_resolution_clock::now();

	auto totalTimeInNs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

	std::cout <<
		"Producers = " << producerCount
		<< ", BatchSize = " << batchSize
		<< ", MessagesPerProducer = " << iterationCount
		<< ", TotalTime = " << totalTimeInNs/1000 << "us"
		<< ", TimePerMessage = " << totalTimeInNs/double(iterationCount * producerCount) << "ns"
		<< ", MessagesPerSecond = " << 1'000'000'000 * (producerCount * iterationCount) / totalTimeInNs;

	constexpr std::uint64_t expectedResult =
		producerCount * std::uint64_t(iterationCount) * std::uint64_t(iterationCount + 1) / 2;

	qor_pp_assert_that(result == expectedResult);
	tp.Shutdown();
}

qor_pp_test_suite_case(MultiProducerSequencerTestSuite, two_producers_single_single_consumer)
{
	ThreadPool tp;
	tp.Setup();
	//static_thread_pool tp{ 3 };

	// Allow time for threads to start up.
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(1ms);

	constexpr std::size_t bufferSize = 16384;

	SequenceBarrier<std::size_t> readBarrier;
	multi_producer_sequencer<std::size_t> sequencer(readBarrier, bufferSize);

	constexpr std::uint64_t iterationCount = 1'000'000;

	std::uint64_t buffer[bufferSize];

	auto startTime = std::chrono::high_resolution_clock::now();

	constexpr std::uint32_t producerCount = 2;
	auto result = std::get<0>(sync_wait(when_all(
		consumer(tp, sequencer, readBarrier, buffer, producerCount),
		one_at_a_time_producer(tp, sequencer, buffer, iterationCount),
		one_at_a_time_producer(tp, sequencer, buffer, iterationCount))));

	auto endTime = std::chrono::high_resolution_clock::now();

	auto totalTimeInNs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

	std::cout <<
		"Producers = " << producerCount
		<< ", NoBatch"
		<< ", MessagesPerProducer = " << iterationCount
		<< ", TotalTime = " << totalTimeInNs / 1000 << "us"
		<< ", TimePerMessage = " << totalTimeInNs / double(iterationCount * producerCount) << "ns"
		<< ", MessagesPerSecond = " << 1'000'000'000 * (producerCount * iterationCount) / totalTimeInNs;

	constexpr std::uint64_t expectedResult =
		producerCount * std::uint64_t(iterationCount) * std::uint64_t(iterationCount + 1) / 2;

	qor_pp_assert_that(result == expectedResult);
	tp.Shutdown();
}

