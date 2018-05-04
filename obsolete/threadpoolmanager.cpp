
/************************************************************************
*    FILE NAME:       threadpoolmanager.cpp
*
*    DESCRIPTION:     Class to manage a thread pool
************************************************************************/

// Physical component dependency
#include <managers/threadpoolmanager.h>

// Boost lib dependencies
//#include <boost/make_shared.hpp>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CThreadPoolMgr::CThreadPoolMgr() //:
    //m_upThreadGroup( new boost::thread_group )
{
    // This has to be created before creating the threads otherwise the threads
    // will fall through and you'll be waiting for threads that don't exist
    /*m_upAsioWork.reset( new boost::asio::io_service::work(m_asioService) );

    // Create a thread pool that with the same number of threads as cores
    for( size_t i = 0; i < boost::thread::hardware_concurrency() ; ++i )
    {
        m_upThreadGroup->create_thread(
            boost::bind( &boost::asio::io_service::run, &m_asioService ));
    }*/
    
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CThreadPoolMgr::~CThreadPoolMgr()
{
    /*m_asioService.stop();
    m_upThreadGroup->join_all();
    m_upAsioWork.reset();
    m_upThreadGroup.reset();*/
    
}   // destructer


/************************************************************************
*    desc:  Post function to work group
************************************************************************/
/*void CThreadPoolMgr::Post( voidFunc func )
{
    typedef boost::packaged_task<void> task_t;
    boost::shared_ptr<task_t> task = boost::make_shared<task_t>(func);
    boost::shared_future<void> fut(task->get_future());
    m_pendingJobs.push_back(fut);
    m_asioService.post(boost::bind(&boost::packaged_task<void>::operator(), task));
    
}*/   // Post


/************************************************************************
*    desc:  Wait for all the jobs to finish
************************************************************************/
/*void CThreadPoolMgr::WaitForAll()
{
    if( !m_pendingJobs.empty() )
    {
        boost::wait_for_all(m_pendingJobs.begin(), m_pendingJobs.end());
        m_pendingJobs.clear();
    }
    
}*/   // WaitForAll

