
/************************************************************************
*    FILE NAME:       smartresolutionbtn.h
*
*    DESCRIPTION:     Class CSmartResBtn
************************************************************************/

#ifndef __smart_resolution_btn_h__
#define __smart_resolution_btn_h__

// Physical component dependency
#include "smartsettingsmenubtn.h"

// Game lib dependencies
#include <common/size.h>

// Standard lib dependencies
#include <vector>

class CSmartResolutionBtn : public CSmartSettingsMenuBtn
{
public:

    // Constructor
    CSmartResolutionBtn( iControl * piControl );

    // Called when the control is created
    void create() override;

    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;

    // Called when the control is executed
    void execute() override;

    // Set the resolution change
    void setResolutionChange();

    // Was the resolution changed
    bool wasResolutionChanged();

private:

    // Size resolution vector
    std::vector< CSize<int> > m_resVec;

    // Resolution index
    int m_resIndex;
};

#endif
