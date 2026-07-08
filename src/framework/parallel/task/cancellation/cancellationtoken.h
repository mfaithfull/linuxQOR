// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_CANCEL_TOKEN
#define QOR_PP_H_FRAMEWORK_TASK_CANCEL_TOKEN

namespace qor
{
	class CancellationSource;
	class CancellationRegistration;

	namespace detail
	{
		class CancellationState;
	}

	class qor_pp_module_interface(QOR_TASK) CancellationToken
	{
	public:

		// Construct to a cancellation token that can't be cancelled.
		CancellationToken() noexcept;

		// Copy another cancellation token.
		// New token will refer to the same underlying state.
		CancellationToken(const CancellationToken& other) noexcept;

		CancellationToken(CancellationToken&& other) noexcept;

		~CancellationToken();

		CancellationToken& operator=(const CancellationToken& other) noexcept;

		CancellationToken& operator=(CancellationToken&& other) noexcept;

		void swap(CancellationToken& other) noexcept;

		// Query if it is possible that this operation will be cancelled or not.
		// Cancellable operations may be able to take more efficient code-paths
		// if they don't need to handle cancellation requests.
		bool can_be_cancelled() const noexcept;

		// Query if some thread has requested cancellation on an associated CancellationSource object.
		bool is_cancellation_requested() const noexcept;

		// Throws operation_cancelled exception if cancellation
		// has been requested for the associated operation.
		void throw_if_cancellation_requested() const;

	private:

		friend class CancellationSource;
		friend class CancellationRegistration;

		CancellationToken(detail::CancellationState* state) noexcept;

		detail::CancellationState* m_state;

	};

	inline void swap(CancellationToken& a, CancellationToken& b) noexcept
	{
		a.swap(b);
	}
}//qor

#endif//QOR_PP_H_FRAMEWORK_TASK_CANCEL_TOKEN
