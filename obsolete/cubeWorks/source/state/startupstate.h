
/************************************************************************
*    FILE NAME:       startupstate.h
*
*    DESCRIPTION:     CStartUp Class State
************************************************************************/

#ifndef __start_up_state_h__
#define __start_up_state_h__

// Physical component dependenc
#include "igamestate.h"

// Standard lib dependencies
#include <string>
#include <mutex>

// Game dependencies
#include "loadscreenanimationinfo.h"

// Forward declaration(s)
class CMatrix;
class CColor;
class CLoadScrnAnim;
struct XMLNode;

class CStartUpState : public iGameState
{
public:

    // Constructor
    CStartUpState();

    // Destructor
    virtual ~CStartUpState(){};

    // Is the state done
    bool DoStateChange();

private:

    // Init the start of the animation
    void InitAnimation( const std::string & group );

    // Load during the startup screen
    void Animate();
    
    // Clean up after the animation is completed
    void CleanUpAnimation( const std::string & group );

    // Load game assets
    void Load();
    
    // Load the assets
    void AssetLoad();
    
    // Load the assets while animating in a thread
    void SynchronousLoad();
    
    // Load the assets with start & end animation
    void AsynchronousLoad();

    // Fade to color
    void Fade(
        CLoadScrnAnim & loadScrnAnim,
        const CColor & cur,
        const CColor & finalColor );
    
    // Init the animation load data
    void LoadAnimData(
        const std::string & group,
        std::vector<CLoadScrnAnim> & animDataVec,
        const XMLNode & node );
    
private:
    
    // mutex for critical section
    std::mutex m_mutex;
    
    // Animation data
    std::vector<CLoadScrnAnim> m_animDataVec;
};

#endif  // __start_up_state_h__


