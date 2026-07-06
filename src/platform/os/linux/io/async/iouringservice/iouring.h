// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURING
#define QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURING

#include <liburing.h>
#include <functional>
#include <system_error>
#include <mutex>
#include <condition_variable>

#include "src/platform/compiler/compiler.h"
#include "src/framework/parallel/sync/asyncmanualresetevent.h"

namespace qor{ namespace io{ namespace async{ namespace lin{

    class qor_pp_module_interface(QOR_LINUXASYNCIOSERVICE) IOUring
    {
    public:

        class CQEIterator;

        class CQE
        {
            friend class CQEIterator;
            friend class IOUring;

        public:

            CQE() : m_(nullptr) {}

            CQE(io_uring_cqe* _) : m_(_) {}

            void* GetData();

            signed int GetResult()
            {
                return m_->res;
            }

        private:
            io_uring_cqe* m_;
        };

        class CQEIterator
        {
        public:

            CQEIterator(io_uring_cqe_iter _) : m_(_) {}
            bool Next(CQE& cqe);

        private:

            io_uring_cqe_iter m_;
        };

        class SQE
        {
        public:

            SQE(const io_uring_sqe* _) : m_(const_cast<io_uring_sqe*>(_)) {}

            void SetData(void* data);
            void PrepareAccept(int fd, struct sockaddr *addr, socklen_t *addrlen, int flags);
            void PrepareBind(int fd, struct sockaddr *addr, socklen_t addrlen);
            void PrepareListen(int fd, int backlog);
            void PrepareRead(int fd, byte* buffer, size_t byteCount, off_t offset);
            void PrepareReadV(int fd,const iovec* iovecs, unsigned int nr_vecs, off_t offset);
            void PrepareWrite(int fd, const byte* buffer, size_t byteCount, off_t offset);
            void PrepareWriteV(int fd, const iovec* iovecs, unsigned int nr_vecs, off_t offset);
            void PrepareSend(int fd, const byte* buffer, size_t len, int flags);
            void PrepareRecv(int fd, byte* byffer, size_t byteCount, int flags);
            void PrepareShutdown(int fd, int how);

        private:
            io_uring_sqe* m_;
        };

        explicit IOUring(size_t queue_size);
        IOUring(const IOUring &) = delete;
        IOUring &operator=(const IOUring &) = delete;
        IOUring(IOUring &&) = delete;
        IOUring &operator=(IOUring &&) = delete;
        ~IOUring();

        struct io_uring *get()
        {
            return &m_ring;
        }

        void Submit() const;
        void RemoteSubmit();
        SQE GetSQE();
        CQEIterator CQEIteratorInit();
        void CQAdvance(unsigned int nr);
        int Peek(IOUring::CQE& temp);
        unsigned int ForEachCQE( std::function<void(IOUring::CQE&)> f);
        int ConsumeCQEntries();
        int ConsumeCQEntriesNonBlocking();
        int ConsumeCQEntries(io_uring_cqe* entries, size_t count);
        unsigned int ExpectationCount() const;

        std::condition_variable m_cond;
        std::mutex m_guard;
    private:

        struct io_uring m_ring;
        std::atomic<unsigned int> m_ExpectationCount;
    };

}}}}//qor::io::async::lin

#endif//QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_IOURING
