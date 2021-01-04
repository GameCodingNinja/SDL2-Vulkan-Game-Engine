
/************************************************************************
*    FILE NAME:       threadpool.h
*
*    DESCRIPTION:     Class to manage a thread pool
************************************************************************/

/* Implementation example
int main()
{
    std::vector< std::future<void> > jobs;
    
    auto & mutex = CThreadPool::Instance().getMutex();
    CThreadPool::Instance().init( 2, 4 );
    
    for( int i = 0; i < 8; ++i )
    {
        jobs.emplace_back(
            CThreadPool::Instance().postRetFut([&mutex] {
                
                std::this_thread::sleep_for(std::chrono::seconds(1));
                
                std::unique_lock<std::mutex> lock( mutex );
                std::cout << "Task finished in thread: " << std::this_thread::get_id() << std::endl;
            })
        );
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Wait for all the jobs to finish
    // get() is a blocking call, waiting for each job to return
    for( auto && iter : jobs ) iter.get();

    return 0;
}
*/

#pragma once

// Standard lib dependencies
#include <vector>
#include <queue>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <stdexcept>
#include <future>
#include <atomic>

// Thread disable flag for testing purposes
//#define __thread_disable__

class CThreadPool
{
public:
    
    static CThreadPool & Instance()
    {
        static CThreadPool threadPool;
        return threadPool;
    }

    // Post to the work queue and return future
    template<typename F, typename... Args>
    auto post(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>;
    
    // Thread pool init
    void init( const int minThreads, const int maxThreads );
    
    // Wait for the jobs to complete
    // NOTE: Only works when the futures are stored internally
    void wait();
    
    // Lock mutex for Synchronization
    void lock();
    
    // Unlock mutex for Synchronization
    void unlock();

    // Stop the thread pool
    void stop();
    
    // Get the mutex
    std::mutex & getMutex();

    // Get the thread count
    size_t threadCount();

    // Is the thread pool active
    bool isActive();

private:
    
    // Constructor
    CThreadPool();
    
    // Destructor
    ~CThreadPool();
    
private:
    
    // need to keep track of threads so we can join them
    std::vector< std::thread> m_threadVec;
    
    // the task queue
    std::queue< std::function<void()> > m_tasks;

    // synchronization
    std::mutex m_queue_mutex;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    
    // Thread pool stop flag
    std::atomic_bool m_stop;
};

/************************************************************************
*    desc:  Post to the work queue and return future
************************************************************************/
template<typename F, typename... Args>
auto CThreadPool::post(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>
{
    using return_type = std::invoke_result_t<F, Args...>;
    
    auto task = std::make_shared < std::packaged_task < return_type()> >(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...) );

    std::future<return_type> res = task->get_future();

    #if defined(__thread_disable__)
    (*task)();
    #else
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);

        // don't allow enqueueing after stopping the pool
        if( m_stop )
            throw std::runtime_error("enqueue on stopped ThreadPool");

        m_tasks.emplace([task]() {
            (*task)(); });
    }
    
    m_condition.notify_one();
    
    #endif

    return res;
}
