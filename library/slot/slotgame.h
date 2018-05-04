
/************************************************************************
*    FILE NAME:       slotgame.h
*
*    DESCRIPTION:     Class for handling the slot game
************************************************************************/

#ifndef __slot_game_h__
#define __slot_game_h__

// Game lib dependencies
#include <slot/slotdefs.h>
#include <slot/slotresults.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <memory>

// Forward declaration(s)
class CSlotGroup;
class CSlotMath;
class CUIMeter;
class CUIControl;
class CMatrix;
class CSymbolSetView;
class iFrontPanel;
class iGameMusic;
class iCycleResults;
class CPlayResult;
union SDL_Event;
struct XMLNode;

class CSlotGame : public boost::noncopyable
{
public:

    // Constructor
    CSlotGame();

    // Destructor
    virtual ~CSlotGame();
    
    // Add the slot group
    void addSlotGroup( std::unique_ptr<CSlotGroup> slotGroup );
    
    // Handle events
    void handleEvent( const SDL_Event & rEvent );
    
    // Update objects that require them
    void update();

    // Transform the game objects
    void transform();

    // Render of game content
    void render( const CMatrix & matrix );
    
    // Play a game
    void playGame(CUIControl *);
    
    // Go through the game state
    void processGameState();
    
    // Set the front panel
    void setFrontPanel( iFrontPanel * pFrontPanel );
    
    // Set the game music
    void setGameMusic( iGameMusic * pGameMusic );
    
    // Get the state
    NSlotDefs::ESlotState getState();
    
    // Do we allow the stop sounds?
    void allowStopSounds( bool allow );
    
    // Create a play result for a slot group
    CPlayResult & createPlayResult();
    
protected:
    
    // Game state functions
    void stateIdle();
    void stateKillCycleResults();
    void statePlaceWager();
    void stateGenerateStops();
    void stateEvaluate();
    void statePreSpin();
    void stateSpin();
    void statePostSpin();
    void statePreAwardWin();
    void stateBonusDecision();
    void statePreBonus();
    void stateBonus();
    void statePostBonus();
    void statePostAwardWin();
    void stateWaitForAward();
    void stateEnd();
    
    // Is the cycle results animating
    bool isCycleResultsAnimating();
    
    // Is the cycle results active
    bool isCycleResultsActive();
    
    // Kill the cycle results
    void killCycleResults();

private:
    
    // Slot results class
    CSlotResults m_slotResults;
    
    // Reel group
    std::vector<std::unique_ptr<CSlotGroup>> m_slotGroupVec;
    
    // slot state
    NSlotDefs::ESlotState m_slotState;
    
    // Class for holding interface items
    // Does not own pointer. Do Not Free
    iFrontPanel * m_pFrontPanel;
    
    // Class for holding interface items
    // Does not own pointer. Do Not Free
    iGameMusic * m_pGameMusic;
};

#endif  // __slot_game_h__


