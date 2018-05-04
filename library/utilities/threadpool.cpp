
/************************************************************************
*    FILE NAME:       threadpool.cpp
*
*    DESCRIPTION:     Class to manage a thread pool
************************************************************************/

// Physical component dependency
#include <utilities/threadpool.h>

// Game lib dependencies
#include <utilities/settings.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CThreadPool::CThreadPool()
{
    #if !defined(__thread_disable__)
    // Get minimum number of threads
    int threads = CSettings::Instance().getMinThreadCount();

    // Get maximum number of threads.
    // Value of zero means use max hardware threads to number of cores
    const int maxThreads = CSettings::Instance().getMaxThreadCount();

    // Get the number of hardware cores. May return 0 if can't determine
    const int maxCores = std::thread::hardware_concurrency();

    // Allow for the maximum number of threads
    if( ((maxThreads == 0) || (maxThreads > maxCores)) && (threads < maxCores) )
        threads = maxCores;

    // Use defined thread count
    else if( (maxThreads > threads) && (maxThreads <= maxCores) )
        threads = maxThreads;

    m_workers.reserve( threads );

    for( int i = 0; i < threads; ++i )
    {
        m_workers.emplace_back(
            [this] {
                for (;;)
                {
                    std::function<void() > task;

                    {
                        std::unique_lock<std::mutex> lock(this->m_queue_mutex);
                        this->m_condition.wait(lock,
                            [this] { return this->m_stop || !this->m_tasks.empty(); });

                        if (this->m_stop && this->m_tasks.empty())
                            return;

                        task = std::move(this->m_tasks.front());
                        this->m_tasks.pop();
                    }

                    task();
                }
            }
        );
    }
    #endif
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CThreadPool::~CThreadPool()
{
    #if !defined(__thread_disable__)
    {
        std::unique_lock<std::mutex> lock( m_queue_mutex );
        m_stop = true;
    }

    m_condition.notify_all();

    for( std::thread & iter : m_workers )
        iter.join();
    #endif
}


/************************************************************************
*    DESC:  Wait for the jobs to complete
************************************************************************/
void CThreadPool::wait()
{
    #if !defined(__thread_disable__)
    // Wait for all the jobs to finish
    // get() is a blocking call, waiting for each job to return
    for( auto && iter : m_jobVec ) iter.get();

    // Clear the vector because all jobs are done
    m_jobVec.clear();
    #endif
}


/************************************************************************
*    DESC:  Lock mutex for Synchronization
************************************************************************/
void CThreadPool::lock()
{
    #if !defined(__thread_disable__)
    m_mutex.lock();
    #endif
}


/************************************************************************
*    DESC:  Unlock mutex for Synchronization
************************************************************************/
void CThreadPool::unlock()
{
    #if !defined(__thread_disable__)
    m_mutex.unlock();
    #endif
}


/************************************************************************
*    DESC:  Get the mutex
************************************************************************/
std::mutex & CThreadPool::getMutex()
{
    return m_mutex;
}
