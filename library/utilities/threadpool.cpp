
/************************************************************************
*    FILE NAME:       threadpool.cpp
*
*    DESCRIPTION:     Class to manage a thread pool
************************************************************************/

// Physical component dependency
#include <utilities/threadpool.h>

// Game lib dependencies
#include <utilities/settings.h>
#include <utilities/genfunc.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CThreadPool::CThreadPool() :
    m_stop(false)
{
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CThreadPool::~CThreadPool()
{
    stop();
}

/************************************************************************
*    DESC:  Thread pool init
************************************************************************/
void CThreadPool::init( const int minThreads, const int maxThreads )
{
    // NOTE: maxThreads value of zero means use max hardware threads to number of cores
    
    #if !defined(__thread_disable__)

    // Get minimum number of threads
    int threads = minThreads;

    // Get the number of hardware cores. May return 0 if can't determine
    const int maxCores = std::thread::hardware_concurrency();

    // Allow for the maximum number of threads
    if( ((maxThreads == 0) || (maxThreads > maxCores)) && (threads < maxCores) )
        threads = maxCores;

    // Use defined thread count
    else if( (maxThreads > threads) && (maxThreads <= maxCores) )
        threads = maxThreads;

    NGenFunc::PostDebugMsg( 
        boost::str( boost::format(
            "Thread Info...\n"
            "  Max cores: %u\n"
            "  Min threads: %u\n"
            "  Max threads: %u\n"
            "  Threads in pool: %u\n" )
            % maxCores
            % minThreads
            % maxThreads
            % threads ));

    m_threadVec.reserve( threads );

    // create all the threads for the pool
    for( int i = 0; i < threads; ++i )
    {
        m_threadVec.emplace_back(
            [this] {
                for (;;)
                {
                    std::function<void() > task;

                    {
                        // Locks this whole section with a mutex
                        std::unique_lock<std::mutex> lock(this->m_queue_mutex);

                        // Wait here until the task queue is not empty or we need to stop the thread pool
                        this->m_condition.wait(lock,
                            [this] { return this->m_stop || !this->m_tasks.empty(); });

                        // Get out now if we are to stop and the task queue is empty
                        if (this->m_stop && this->m_tasks.empty())
                            return;

                        // Get the top most task and pop it from the queue to be executed
                        task = std::move(this->m_tasks.front());
                        this->m_tasks.pop();
                    }

                    // Execute the task
                    task();
                }
            }
        );
    }
    #endif
}

/************************************************************************
*    DESC:  Wait for the jobs to complete
*           NOTE: Only works when the futures are stored internally
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
*    DESC:  Stop the thread
************************************************************************/
void CThreadPool::stop()
{
    if( !m_stop )
    {
        #if !defined(__thread_disable__)
        {
            std::unique_lock<std::mutex> lock( m_queue_mutex );
            m_stop = true;
        }

        m_condition.notify_all();

        for( std::thread & iter : m_threadVec )
            iter.join();
        #endif

        m_jobVec.clear();
        m_tasks = std::queue< std::function<void()> >();
        m_threadVec.clear();
    }
}

/************************************************************************
*    DESC:  Get the mutex
************************************************************************/
std::mutex & CThreadPool::getMutex()
{
    return m_mutex;
}

/************************************************************************
*    DESC:  Get the thread count
************************************************************************/
size_t CThreadPool::threadCount()
{
    return m_threadVec.size();
}
