// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_CANCEL_REGISTRATION
#define QOR_PP_H_FRAMEWORK_TASK_CANCEL_REGISTRATION

#include <functional>
#include <utility>
#include <type_traits>
#include <atomic>
#include <cstdint>

#include "cancellationtoken.h"

namespace qor{

	namespace detail{
		class CancellationState;
		struct CancellationRegistrationListChunk;
		struct CancellationRegistrationState;
	}//qor::detail

	class qor_pp_module_interface(QOR_TASK) CancellationRegistration
	{
	public:

		/// Registers the callback to be executed when cancellation is requested
		/// on the CancellationToken.
		///
		/// The callback will be executed if cancellation is requested for the
		/// specified cancellation token. If cancellation has already been requested
		/// then the callback will be executed immediately, before the constructor
		/// returns. If cancellation has not yet been requested then the callback
		/// will be executed on the first thread to request cancellation inside
		/// the call to CancellationSource::request_cancellation().
		///
		/// \param token
		/// The cancellation token to register the callback with.
		///
		/// \param callback
		/// The callback to be executed when cancellation is requested on the
		/// the CancellationToken. Note that callback must not throw an exception
		/// if called when cancellation is requested otherwise std::terminate()
		/// will be called.
		///
		/// \throw std::bad_alloc
		/// If registration failed due to insufficient memory available.
		template< typename FUNC, typename = std::enable_if_t<std::is_constructible_v<std::function<void()>, FUNC&&>>>
		CancellationRegistration(CancellationToken token, FUNC&& callback) : m_callback(std::forward<FUNC>(callback))
		{
			register_callback(std::move(token));
		}

		CancellationRegistration(const CancellationRegistration& other) = delete;
		CancellationRegistration& operator=(const CancellationRegistration& other) = delete;

		/// Deregisters the callback.
		///
		/// After the destructor returns it is guaranteed that the callback
		/// will not be subsequently called during a call to request_cancellation()
		/// on the CancellationSource.
		///
		/// This may block if cancellation has been requested on another thread
		/// is it will need to wait until this callback has finished executing
		/// before the callback can be destroyed.
		~CancellationRegistration();

	private:

		friend class detail::CancellationState;
		friend struct detail::CancellationRegistrationState;

		void register_callback(CancellationToken&& token);

		detail::CancellationState* m_state;
		std::function<void()> m_callback;
		detail::CancellationRegistrationListChunk* m_chunk;
		std::uint32_t m_entryIndex;
	};

}//qor

#endif//QOR_PP_H_FRAMEWORK_TASK_CANCEL_REGISTRATION
