
/************************************************************************
*    FILE NAME:       reelgroupview.h
*
*    DESCRIPTION:     View class for the reel group
************************************************************************/

#ifndef __reel_group_view_h__
#define __reel_group_view_h__

// Physical component dependency
#include <slot/slotgroupview.h>

// Game lib dependencies
#include <slot/slotdefs.h>
#include <slot/reelstripview.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <deque>
#include <vector>

// Forward declaration(s)
class CSymbolSetView;
class CSlotStripModel;
class CSymbol2d;
class CMatrix;

class CReelGroupView : public CSlotGroupView, boost::noncopyable
{
public:

    // Constructor
    CReelGroupView( const std::shared_ptr<CSlotGroupModel> & rSlotGroupModel );

    // Destructor
    virtual ~CReelGroupView();
    
    // Create the view slot strips
    void create(
        const XMLNode & node,
        CSymbolSetView & rSymbolSetView,
        std::unique_ptr<iCycleResults> upCycleResults ) override;
    
    // Update the reel group
    void update() override;
    
    // Transform the reel group
    void transform() override;
    
    // Do the render
    void render( const CMatrix & matrix ) override;
    
    // Do the deferred render
    void deferredRender( const CMatrix & matrix ) override;
    
    // Start the reels spinning
    void startSpin() override;
    
    // Stop the reels spinning
    void stopSpin() override;
    
    // Is the spin state
    bool isSpinState( NSlotDefs::ESpinState state ) const override;
    
    // Generate the cycle results symbols
    void generateCycleResultSymbs() override;
    
    // Get the cycle results symbols
    std::vector<std::vector<CSymbol2d *>> & getCycleResultSymbs() override;
    
    // Clear the cycle results symbols
    void clearCycleResultSymbs() override;
    
    // Do we allow the stop sounds?
    void allowStopSounds( bool allow ) override;
    
private:
    
    // Free the memory allocated for the cycle result symbols
    void deleteCycleResultSymbs();
    
private:
    
    // Deque of reel strip views
    std::deque<CReelStripView> m_reelStripViewDeq;
    
    // Cycle result symb vectors
    std::vector<std::vector<CSymbol2d *>> m_cycleResultSymbVec;
};

#endif  // __reel_group_view_h__
