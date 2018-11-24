
/************************************************************************
*    FILE NAME:       build_defs.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include <common/build_defs.h>

// Uncomment this to visually to develop the mobal version files on the PC
//#define DEBUG_MOBILE 1

// Uncomment this to be in debug mode
#define DEBUG 1

namespace NBDefs
{
    bool IsMobileDevice()
    {
        #if defined(__IOS__) || defined(__ANDROID__) || defined(DEBUG_MOBILE)
        return true;
        #else
        return false;
        #endif
    }
    
    bool IsDebugMode()
    {
        #ifdef DEBUG
        return true;
        #else
        return false;
        #endif
    }
    
}
