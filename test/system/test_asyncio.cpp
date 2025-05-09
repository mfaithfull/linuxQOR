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

#include "../../src/configuration/configuration.h"

#include <algorithm>
#include <coroutine>
#include <filesystem>
#include <iostream>
#include <sys/ioctl.h>
#include <utility>
#include <vector>
#include <cassert>
#include <liburing.h>

#include "src/qor/test/test.h"
#include "src/qor/assert/assert.h"
#include "src/qor/module/moduleregistry.h"
#include "src/qor/injection/typeidentity.h"
#include "src/qor/objectcontext/anyobject.h"
#include "src/framework/thread/currentthread.h"
#include "src/framework/thread/thread.h"
#include "src/framework/thread/threadpool.h"
#include "src/framework/thread/currentprocess.h"
#include "src/qor/reference/newref.h"
#include "src/system/filesystem/filesystem.h"
#include "src/system/filesystem/folder.h"


using namespace qor;
using namespace qor::test;
using namespace qor::system;
using namespace qor::framework;


struct AsyncIOTestSuite{};

qor_pp_test_suite_case(AsyncIOTestSuite, EXPERIMENTAL)
{

}

off_t get_file_size(int fd) 
{
    struct stat st;

    if (fstat(fd, &st) < 0) {
        perror("fstat");
        return -1;
    }

    if (S_ISBLK(st.st_mode)) {
        unsigned long long bytes;
        if (ioctl(fd, BLKGETSIZE64, &bytes) != 0) {
        perror("ioctl");
        return -1;
        }
        return bytes;
    } else if (S_ISREG(st.st_mode))
        return st.st_size;

    return -1;
}

constexpr auto MAX_FILES = 512ul;

class ReadOnlyFile {
    public:
      ReadOnlyFile(const std::string &file_path) : path_{file_path} {
        fd_ = open(file_path.c_str(), O_RDONLY);
        if (fd_ < 0) {
          throw std::runtime_error("Could not open file: " + file_path);
        }
        size_ = get_file_size(fd_);
        if (size_ < 0) {
          throw std::runtime_error("Could not estimate size of file");
        }
      }
      ReadOnlyFile(ReadOnlyFile &&other)
          : path_{std::exchange(other.path_, {})},
            fd_{std::exchange(other.fd_, -1)}, size_{other.size()} {};
    
      ~ReadOnlyFile() {
        if (fd_) {
          close(fd_);
        }
      }
      int fd() const { return fd_; }
      off_t size() const { return get_file_size(fd_); }
      const std::string &path() const { return path_; }
    
    private:
      std::string path_;
      int fd_;
      off_t size_;
};

struct Result {
    //tinyobj::ObjReader result; // stores the actual parsed obj
    int status_code{0};        // the status code of the read operation
    std::string file;
  };

class IOUring 
{
public:

    explicit IOUring(size_t queue_size) 
    {
        if (auto s = io_uring_queue_init(queue_size, &ring_, 0); s < 0) 
        {
            throw std::runtime_error("error: " + std::to_string(s));
        }
    }

    IOUring(const IOUring &) = delete;
    IOUring &operator=(const IOUring &) = delete;
    IOUring(IOUring &&) = delete;
    IOUring &operator=(IOUring &&) = delete;

    ~IOUring() 
    { 
        io_uring_queue_exit(&ring_); 
    }
    
    struct io_uring *get() 
    {
        return &ring_;
    }
    
private:

    struct io_uring ring_;
};

class Task 
{
    public:

      struct promise_type 
      {
        Result result;
    
        Task get_return_object() { return Task(this); }    
        void unhandled_exception() noexcept {}
        void return_value(Result result) noexcept { result = std::move(result); }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
      };
    
      explicit Task(promise_type *promise) : handle_{HandleT::from_promise(*promise)} {}
      Task(Task &&other) : handle_{std::exchange(other.handle_, nullptr)} {}
    
      ~Task() 
      {
        if (handle_) {
          handle_.destroy();
        }
      }
    
      Result getResult() const & 
      {
        assert(handle_.done());
        return handle_.promise().result;
      }
    
      Result &&getResult() && 
      {
        assert(handle_.done());
        return std::move(handle_.promise().result);
      }
    
      bool done() const { return handle_.done(); }
    
      using HandleT = std::coroutine_handle<promise_type>;
      HandleT handle_;
};

struct RequestData 
{
    std::coroutine_handle<> handle;
    int statusCode{-1};
};

class ReadFileAwaitable 
{
    public:

    ReadFileAwaitable(IOUring &uring, const ReadOnlyFile &file, std::vector<char> &buf) 
    {
        sqe_ = io_uring_get_sqe(uring.get());
        io_uring_prep_read(sqe_, file.fd(), buf.data(), buf.size(), 0);
    }
    
    auto operator co_await() 
    {
        struct Awaiter 
        {
            io_uring_sqe *entry;
            RequestData requestData;
            Awaiter(io_uring_sqe *sqe) : entry{sqe} {}
            bool await_ready() { return false; }
            void await_suspend(std::coroutine_handle<> handle) noexcept 
            {
                requestData.handle = handle;
                io_uring_sqe_set_data(entry, &requestData);
            }
            int await_resume() { return requestData.statusCode; }
        };
        return Awaiter{sqe_};
    }
    
private:
    io_uring_sqe* sqe_;
};

class TestThreadPool 
{
    public:

      TestThreadPool()
      {
        pool_.Setup();
      }
      
      auto Schedule() 
      {
        struct Awaiter : std::suspend_always 
        {
            ThreadPool &tpool;
            Awaiter(ThreadPool &pool) : tpool{pool} {}
            void await_suspend(std::coroutine_handle<> handle) 
            {
                tpool.PostTask([handle, this]() { handle.resume(); });
            }
        };
        return Awaiter{pool_};
      }
    
      size_t numUnfinishedTasks() const { return pool_.GetTotalCountOfTasks(); }
    
    private:
      ThreadPool pool_;
};

int consumeCQEntries(IOUring &uring) 
{
    int processed{0};
    io_uring_cqe *cqe;
    unsigned head;
    io_uring_for_each_cqe(uring.get(), head, cqe) {
      auto *request_data = static_cast<RequestData *>(io_uring_cqe_get_data(cqe));
      request_data->statusCode = cqe->res;
      request_data->handle.resume();
      ++processed;
    }
    io_uring_cq_advance(uring.get(), processed);
    return processed;
}

int consumeCQEntriesNonBlocking(IOUring &uring) 
{
    io_uring_cqe *temp;
    if (io_uring_peek_cqe(uring.get(), &temp) == 0) {
      return consumeCQEntries(uring);
    }
    return 0;
}

std::vector<Result> gatherResults(const std::vector<Task> &tasks) {
    std::vector<Result> results;
    results.reserve(tasks.size());
    for (auto &&t : tasks) {
      results.push_back(std::move(t).getResult());
    }
    return results;
}

Task parseOBJFile(IOUring &uring, const ReadOnlyFile &file, TestThreadPool &pool) {
    std::vector<char> buff(file.size());
    int status = co_await ReadFileAwaitable(uring, file, buff);
    co_await pool.Schedule();
    Result result{.status_code = 0, .file = file.path()};
    //readObjFromBuffer(buff, result.result);
    co_return result;
}
  
  bool allDone(const std::vector<Task> &tasks) 
  {
    return std::all_of(tasks.cbegin(), tasks.cend(), [](const auto &t) 
        {
            return t.done(); 
        }
    );
  }

std::vector<Result> coroutines_pool(const std::vector<ReadOnlyFile> &files) 
{
    IOUring uring{files.size()};
    TestThreadPool pool;
    std::vector<Task> tasks;
    tasks.reserve(files.size());
    for (const auto &file : files) {
        tasks.push_back(parseOBJFile(uring, file, pool));
    }
    io_uring_submit(uring.get());
    while (pool.numUnfinishedTasks() > 0 || !allDone(tasks)) 
    {
        // consume entries in the completion queue
        // return immediately if the queue is empty
        consumeCQEntriesNonBlocking(uring);
    }

    return gatherResults(tasks);
}