/************************************************************************
*    FILE NAME:       exceptionhandling.h
*
*    DESCRIPTION:     Exception handling classes
************************************************************************/           

#pragma once

// Standard lib dependencies
#include <string>

namespace NExcept
{
    // Interface exception class
    class IException
    {
    public:
        
        IException( std::string title, std::string msg )
        {
            // This will keep additional exceptions
            // from overwriting the first one
            if( errorTitle.empty() )
                errorTitle = title;
            
            if( errorMsg.empty() )
                errorMsg = msg;
        }

        // Return the error info
        std::string & getErrorMsg(){ return errorMsg; }
        std::string & getErrorTitle(){ return errorTitle; }

    protected:

        static std::string errorTitle;
        static std::string errorMsg;
    };

    // Critical exception class. Used for non-recoverable exceptions
    class CCriticalException : public IException
    {
    public:
        
        CCriticalException( std::string title, std::string msg ) : IException(title, msg) {}
    };
}
