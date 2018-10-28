
/************************************************************************
*    FILE NAME:       wheelgroupview.h
*
*    DESCRIPTION:     Class for wheel group view
************************************************************************/

#ifndef __wheel_group_view_h__
#define __wheel_group_view_h__

// Physical component dependency
#include <slot/slotgroupview.h>

// Game lib dependencies
#include <slot/slotdefs.h>
#include <slot/wheelview.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <deque>

// Forward declaration(s)
class CSymbolSetView;
class CSlotStripModel;
class CSymbol2d;
class CMatrix;

class CWheelGroupView : public CSlotGroupView, boost::noncopyable
{
public:

    // Constructor
    CWheelGroupView( const std::shared_ptr<CSlotGroupModel> & rSlotGroupModel );

    // Destructor
    virtual ~CWheelGroupView();
    
    // Create the view slot strips
    void create(
        const XMLNode & node,
        CSymbolSetView & rSymbolSetView,
        std::unique_ptr<iCycleResults> upCycleResults ) override;
    
    // Update the reel group
    void update() override;
    
    // Transform the reel group
    void transform() override;
    
    // do the render
    //void render( const CMatrix & matrix ) override;
    
    // Do the deferred render
    //void deferredRender( const CMatrix & matrix ) override;
    
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
    
    // Do we allow the stop sounds?
    void allowStopSounds( bool allow ) override;
    
private:

    // Deque of wheel views
    std::deque<CWheelView> m_wheelViewDeq;
    
    // Cycle result symb vectors
    std::vector<std::vector<CSymbol2d *>> m_cycleResultSymbVec;
};

#endif
