
/************************************************************************
*    FILE NAME:       smartresolutionbtn.h
*
*    DESCRIPTION:     Class CSmartResBtn
************************************************************************/

#ifndef __smart_resolution_btn_h__
#define __smart_resolution_btn_h__

// Physical component dependency
#include "smartsettingsmenubtn.h"

// Standard lib dependencies
#include <vector>

// Game lib dependencies
#include <common/size.h>

class CSmartResolutionBtn : public CSmartSettingsMenuBtn
{
public:

    // Constructor
    CSmartResolutionBtn( CUIControl * pUIControl );

    // Called when the control is created
    void Create();

    // Handle events
    void HandleEvent( const SDL_Event & rEvent );

    // Called when the control is executed
    void Execute();

    // Set the resolution change
    void SetResolutionChange();

    // Was the resolution changed
    bool WasResolutionChanged();

private:

    // Size resolution vector
    std::vector< CSize<int> > m_resVec;

    // Resolution index
    int m_resIndex;

};

#endif  // __smart_resolution_btn_h__


