// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_CANCEL_STATE
#define QOR_PP_H_FRAMEWORK_TASK_CANCEL_STATE

#include <thread>
#include <atomic>
#include <cstdint>

#include "cancellationtoken.h"

namespace qor{	namespace detail{

	struct CancellationRegistrationState;

	class qor_pp_module_interface(QOR_TASK) CancellationState
	{
	public:

		/// Allocates a new CancellationState object.
		///
		/// \throw std::bad_alloc
		/// If there was insufficient memory to allocate one.
		static CancellationState* create();

		~CancellationState();

		/// Increment the reference count of CancellationToken and
		/// CancellationRegistration objects referencing this state.
		void add_token_ref() noexcept;

		/// Decrement the reference count of CancellationToken and
		/// CancellationRegistration objects referencing this state.
		void release_token_ref() noexcept;

		/// Increment the reference count of CancellationSource objects.
		void add_source_ref() noexcept;

		/// Decrement the reference count of cancellation_souce objects.
		///
		/// The CancellationState will no longer be cancellable once the
		/// CancellationSource ref count reaches zero.
		void release_source_ref() noexcept;

		/// Query if the CancellationState can have cancellation requested.
		///
		/// \return
		/// Returns true if there are no more references to a CancellationSource
		/// object.
		bool can_be_cancelled() const noexcept;

		/// Query if some thread has called request_cancellation().
		bool is_cancellation_requested() const noexcept;

		/// Flag state has having cancellation_requested and execute any
		/// registered callbacks.
		void request_cancellation();

		/// Try to register the CancellationRegistration as a callback to be executed
		/// when cancellation is requested.
		///
		/// \return
		/// true if the callback was successfully registered, false if the callback was
		/// not registered because cancellation had already been requested.
		///
		/// \throw std::bad_alloc
		/// If callback was unable to be registered due to insufficient memory.
		bool try_register_callback(CancellationRegistration* registration);

		/// Deregister a callback previously registered successfully in a call to try_register_callback().
		///
		/// If the callback is currently being executed on another
		/// thread that is concurrently calling request_cancellation()
		/// then this call will block until the callback has finished executing.
		void deregister_callback(CancellationRegistration* registration) noexcept;

	private:

		CancellationState() noexcept;

		bool is_cancellation_notification_complete() const noexcept;

		static constexpr std::uint64_t cancellation_requested_flag = 1;
		static constexpr std::uint64_t cancellation_notification_complete_flag = 2;
		static constexpr std::uint64_t cancellation_source_ref_increment = 4;
		static constexpr std::uint64_t cancellation_token_ref_increment = UINT64_C(1) << 33;
		static constexpr std::uint64_t can_be_cancelled_mask = cancellation_token_ref_increment - 1;
		static constexpr std::uint64_t cancellation_ref_count_mask = ~(cancellation_requested_flag | cancellation_notification_complete_flag);

		// A value that has:
		// - bit 0 - indicates whether cancellation has been requested.
		// - bit 1 - indicates whether cancellation notification is complete.
		// - bits 2-32 - ref-count for CancellationSource instances.
		// - bits 33-63 - ref-count for CancellationToken/CancellationRegistration instances.
		std::atomic<std::uint64_t> m_state;

		std::atomic<CancellationRegistrationState*> m_registrationState;

	};

}}//qor::detail

#endif//QOR_PP_H_FRAMEWORK_TASK_CANCEL_STATE
