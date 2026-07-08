// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_CANCEL_SOURCE
#define QOR_PP_H_FRAMEWORK_TASK_CANCEL_SOURCE

namespace qor
{
	class CancellationToken;

	namespace detail
	{
		class CancellationState;
	}

	class qor_pp_module_interface(QOR_TASK) CancellationSource
	{
	public:

		/// Construct to a new cancellation source.
		CancellationSource();

		/// Create a new reference to the same underlying cancellation
		/// source as \p other.
		CancellationSource(const CancellationSource& other) noexcept;

		CancellationSource(CancellationSource&& other) noexcept;

		~CancellationSource();

		CancellationSource& operator=(const CancellationSource& other) noexcept;

		CancellationSource& operator=(CancellationSource&& other) noexcept;

		/// Query if this cancellation source can be cancelled.
		///
		/// A cancellation source object will not be cancellable if it has
		/// previously been moved into another CancellationSource instance
		/// or was copied from a CancellationSource that was not cancellable.
		bool can_be_cancelled() const noexcept;

		/// Obtain a cancellation token that can be used to query if
		/// cancellation has been requested on this source.
		///
		/// The cancellation token can be passed into functions that you
		/// may want to later be able to request cancellation.
		CancellationToken token() const noexcept;

		/// Request cancellation of operations that were passed an associated
		/// cancellation token.
		///
		/// Any cancellation callback registered via a CancellationRegistration
		/// object will be called inside this function by the first thread to
		/// call this method.
		///
		/// This operation is a no-op if can_be_cancelled() returns false.
		void request_cancellation();

		/// Query if some thread has called 'request_cancellation()' on this
		/// CancellationSource.
		bool is_cancellation_requested() const noexcept;

	private:

		detail::CancellationState* m_state;

	};

}//qor

#endif//QOR_PP_H_FRAMEWORK_TASK_CANCEL_SOURCE
