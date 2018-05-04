
/************************************************************************
*    FILE NAME:       threadpoolmanager.h
*
*    DESCRIPTION:     Class to manage a thread pool
************************************************************************/

#ifndef __thread_pool_mamager_h__
#define __thread_pool_mamager_h__

// Boost lib dependencies
//#include <boost/asio.hpp>
//#include <boost/thread.hpp>
//#include <boost/bind.hpp>

class CThreadPoolMgr
{
public:
    
    // Get the instance of the singleton class
    /*static CThreadPoolMgr & Instance()
    {
        static CThreadPoolMgr threadPoolMgr;
        return threadPoolMgr;
    }
    
    typedef boost::function<void()> voidFunc;
    
    // Post function to work group
    void Post( voidFunc func );
    
    // Wait for all the jobs to finish
    void WaitForAll();*/

private:
    
    // Constructor
    CThreadPoolMgr();

    // Destructor
    virtual ~CThreadPoolMgr();
    
private:
    /*
    // Boost ASIO service
    boost::asio::io_service m_asioService;

    // Boost ASIO service work
    std::unique_ptr<boost::asio::io_service::work> m_upAsioWork;

    // Boost thread group
    std::unique_ptr<boost::thread_group> m_upThreadGroup;

    // Vector of current jobs
    std::vector<boost::shared_future<void> > m_pendingJobs;
*/
};

#endif  // __thread_pool_mamager_h__


