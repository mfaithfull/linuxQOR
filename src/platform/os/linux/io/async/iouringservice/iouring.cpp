// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <stdexcept>

#include "iouring.h"
#include "src/framework/io/async/service.h"
#include "src/qor/flyers/log/informative.h"

namespace qor{ namespace io{ namespace async{ namespace lin{

    IOUring::IOUring(size_t queue_size)
    {
        m_ExpectationCount = 0;
        if (auto s = io_uring_queue_init(queue_size, &m_ring, 0); s < 0)
        {
            throw std::runtime_error("error: " + std::to_string(s));
        }
    }

    IOUring::SQE IOUring::GetSQE()
    {
        IOUring::SQE sqe( io_uring_get_sqe(const_cast<io_uring*>(&m_ring)));
        return sqe;
    }

    IOUring::CQEIterator IOUring::CQEIteratorInit()
    {
        IOUring::CQEIterator cqe_it(io_uring_cqe_iter_init(&m_ring));
        return cqe_it;
    }

    void IOUring::CQAdvance(unsigned int nr)
    {
        io_uring_cq_advance(&m_ring, nr);
    }

    int IOUring::Peek(IOUring::CQE& temp)
    {
        return io_uring_peek_cqe(&m_ring, &temp.m_);
    }

    unsigned int IOUring::ForEachCQE(std::function<void(IOUring::CQE&)> f)
    {
        unsigned int processed{0};
        CQE cqe;
        auto it = CQEIteratorInit();
        while(it.Next(cqe))
        {
            f(cqe);
            ++processed;
        };
        CQAdvance(processed);
        return processed;
    }

    void IOUring::Submit() const
    {
        io_uring_submit(const_cast<io_uring*>(&m_ring));
    }

    void IOUring::RemoteSubmit()
    {
        m_ExpectationCount.fetch_add(1, std::memory_order_relaxed);
        //sem.release(1);
        m_cond.notify_one();
    }

    unsigned int IOUring::ExpectationCount() const
    {
        return m_ExpectationCount.load(std::memory_order_acquire);
    }

    IOUring::~IOUring()
    {
        std::unique_lock<std::mutex> lock(m_guard);
        io_uring_queue_exit(&m_ring);
    }

    void IOUring::SQE::PrepareAccept(int fd, struct sockaddr *addr, socklen_t *addrlen, int flags)
    {
        io_uring_prep_accept(m_, fd, addr, addrlen, flags);
    }

    void IOUring::SQE::PrepareBind(int fd, struct sockaddr *addr, socklen_t addrlen)
    {
        io_uring_prep_bind(m_, fd, addr, addrlen);
    }

    void IOUring::SQE::PrepareListen(int fd, int backlog)
    {
        io_uring_prep_listen(m_, fd, backlog);
    }

    void IOUring::SQE::PrepareRead(int fd, byte* buffer, size_t byteCount, off_t offset)
    {
        io_uring_prep_read(m_, fd, buffer, byteCount, offset);
    }

    void IOUring::SQE::PrepareReadV(int fd,const iovec* iovecs, unsigned int nr_vecs, off_t offset )
    {
        io_uring_prep_readv(m_, fd, iovecs, nr_vecs, offset);
    }

    void IOUring::SQE::PrepareWrite(int fd, const byte* buffer, size_t byteCount, off_t offset)
    {
        io_uring_prep_write(m_, fd, buffer, byteCount, offset);
    }

    void IOUring::SQE::PrepareWriteV(int fd, const iovec* iovecs, unsigned int nr_vecs, off_t offset)
    {
        io_uring_prep_writev(m_, fd, iovecs, nr_vecs, offset);
    }

    void IOUring::SQE::PrepareSend(int fd, const byte* buffer, size_t len, int flags)
    {
        io_uring_prep_send(m_, fd, buffer, len, flags);
    }

    void IOUring::SQE::PrepareRecv(int fd, byte* buffer, size_t byteCount, int flags)
    {
        io_uring_prep_recv(m_, fd, buffer, byteCount, flags);
    }

    void IOUring::SQE::PrepareShutdown(int fd, int how)
    {
        io_uring_prep_shutdown(m_, fd, how);
    }

    void IOUring::SQE::SetData(void* data)
    {
        io_uring_sqe_set_data(m_, data);
    }

    bool IOUring::CQEIterator::Next(CQE& cqe)
    {
        return io_uring_cqe_iter_next(&m_, &cqe.m_);
    }

    void* IOUring::CQE::GetData()
    {
        return io_uring_cqe_get_data(m_);
    }

    int IOUring::ConsumeCQEntries()
    {
        return ForEachCQE( [this](IOUring::CQE& cqe){
            auto *request_data = static_cast<qor::io::async::Request*>(cqe.GetData());
            if(request_data)
            {
                request_data->statusCode = cqe.GetResult();
                if(!request_data->handle.done())
                {
                    request_data->handle.resume();
                }
                else
                {
                    log::inform("Already done {0}", cqe.GetResult());
                }
                m_ExpectationCount.fetch_sub(1, std::memory_order_relaxed);
            }
        });
    }

    int IOUring::ConsumeCQEntriesNonBlocking()
    {
        IOUring::CQE temp;
        if (Peek(temp) == 0) {
          return ConsumeCQEntries();
        }
        return 0;
    }

    int IOUring::ConsumeCQEntries(io_uring_cqe* entries, size_t count)
    {
        unsigned int processed{0};

        io_uring_cqe* cqe = entries;
        size_t index = 0;
        for(; index < count; ++index )
        {
            auto *request_data = static_cast<qor::io::async::Request*>(io_uring_cqe_get_data(cqe));
            if(request_data)
            {
                request_data->statusCode = cqe->res;
                m_ExpectationCount.fetch_sub(1, std::memory_order_relaxed);
                if(!request_data->handle.done())
                {
                    request_data->handle.resume();
                }
                else
                {
                    log::inform("Already done {0}", cqe->res);
                }
                ++processed;
                ++cqe;
            }
            else
            {
                ++processed;
                ++cqe;
            }
        };
        CQAdvance(processed);
        return processed;
    }

}}}}//qor::io::async::lin
