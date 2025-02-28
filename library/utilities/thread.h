
/************************************************************************
*    FILE NAME:       thread.h
*
*    DESCRIPTION:     CThread Class
************************************************************************/

#pragma once

// Game lib dependencies
#include <utilities/deletefuncs.h>
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>

// SDL lib dependencies
#include <SDL3/SDL.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>

namespace thread
{
    // Use an odd number for the exit code so that it can
    // be spotted in the debug window when the thread exits
    enum{ EXIT_CODE = 12345 };

    template<class T>
    class CThread : public boost::noncopyable
    {
        // new type Method: pointer to a object's method (this call)
        typedef int (T::* Method)();

    public:

        // Creates a new Thread object. object: the one which method should be
        // executed. method: pointer to the object's method.
        CThread() : m_pThread(NULL), m_threadID(0)
        { }

        // Destructor
        ~CThread()
        { }

        // Starts executing the objects method in a concurrent thread.
        void start(T* object, int ( T::* method)(), const char* pThreadname, bool detachThread = false)
        {
            // Storing the name of the thread so that it's pointer never goes out of scope.
            // This solved a random problem where the name was being passed as a hard coded
            // string passed through the Start function but when out of scope causng an assert. 
            m_threadName = pThreadname;

            m_object = object;
            m_method = method;
            m_detachThread = detachThread;
            m_threadRunning = false;
            m_threadID = 0;

            if( Running() )
                throw NExcept::CCriticalException("Thread Create Error!",
                    "Can't start because thread already running!");

            // Set this early so that is can be queried as soon as this function returns
            m_threadRunning = true;

            if( (m_pThread = SDL_CreateThread((SDL_ThreadFunction) CThread<T>::run, m_threadName.c_str(), this)) == NULL )
                NGenFunc::PostDebugMsg( "Thread Create Error! - " + std::string(SDL_GetError()) );

            if( (m_threadID = SDL_GetThreadID( m_pThread )) == 0 )
                NGenFunc::PostDebugMsg( "Thread ID Error! - " + std::string(SDL_GetError()) );

            // Detach so we don't have to do any clean-up
            if( m_detachThread )
                SDL_DetachThread( m_pThread );
        }

        // Has the thread started
        bool started()
        {
            if( m_threadID > 0 )
                return true;
            
            return false;
        }

        // Is the thread running
        bool running()
        {
            return ( Started() && m_threadRunning );
        }

        // Wait for the thread to finish
        void waitForThread()
        {
            if( !m_detachThread && Running() )
                SDL_WaitThread( m_pThread, NULL );
        }

    private:

        // thread pointer
        SDL_Thread * m_pThread;

        // unique ID to the thread
        SDL_threadID m_threadID;

        // the object which owns the method
        T * m_object;   

        // the method of the object
        Method m_method;

        // Detach flag
        bool m_detachThread;

        // Thread running flag
        static bool m_threadRunning;

        // Thread name
        std::string m_threadName;

    private:

        // This function gets executed by a concurrent thread.
        static int run(void * thread_obj)
        {
            CThread<T>* thread = (CThread<T>*)thread_obj;
            int exitCode = (thread->m_object->*thread->m_method)();

            m_threadRunning = false;

            return exitCode;
        }

    };

    template<class T> bool CThread<T>::m_threadRunning = false;

}
