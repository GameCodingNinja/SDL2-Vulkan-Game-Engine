
/************************************************************************
*    FILE NAME:       slotgroupview.h
*
*    DESCRIPTION:     View base class for the slot group
************************************************************************/

#ifndef __slot_group_view_h__
#define __slot_group_view_h__

// Physical component dependency
#include <2d/object2d.h>

// Game lib dependencies
#include <slot/spinprofile.h>
#include <slot/slotdefs.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>
#include <memory>

// Forward declaration(s)
struct XMLNode;
class CSlotGroupModel;
class CSymbolSetView;
class CSymbol2d;
class CSprite;
class iCycleResults;
class CPay;

class CSlotGroupView : public CObject2D
{
public:

    // Constructor
    CSlotGroupView( const std::shared_ptr<CSlotGroupModel> & rSlotGroupModel );

    // Destructor
    virtual ~CSlotGroupView();
    
    // Create the view slot strips
    virtual void create(
        const XMLNode & node,
        CSymbolSetView & rSymbolSetView,
        std::unique_ptr<iCycleResults> upCycleResults );
    
    // Set the cycle results text
    void setCycleResultText( bool visible, const CPay * pPay = nullptr );
    
    // Load the spin profile from XML node
    void loadSpinProfileFromNode( const XMLNode & node );
    
    // Clear the cycle results symbols
    virtual void clearCycleResultSymbs();
    
    // Generate the cycle results symbols
    virtual void generateCycleResultSymbs() = 0;

    // Get the cycle results symbols
    virtual std::vector<std::vector<CSymbol2d *>> & getCycleResultSymbs() = 0;
    
    // Do we allow the stop sounds?
    virtual void allowStopSounds( bool allow ) = 0;
    
    // Update the reel group
    virtual void update();
    
    // Transform the reel group
    virtual void transform();
    
    // Do the render
    //virtual void render( const CMatrix & matrix );
    
    // Do the deferred render
    //virtual void deferredRender( const CMatrix & matrix ) = 0;
    
    // Start the reels spinning
    virtual void startSpin() = 0;
    
    // Stop the reels spinning
    virtual void stopSpin() = 0;
    
    // Is the spin state
    virtual bool isSpinState( NSlotDefs::ESpinState state ) const = 0;
    
    // Activate the cycle results
    void activateCycleResults();
    
    // Stop the cycle results
    void deactivateCycleResults();
    
    // Start the cycle results animation
    void startCycleResultsAnimation();
    
    // Stop the cycle results animation
    void stopCycleResultsAnimation();
    
    // Is the cycle results active
    bool isCycleResultsActive();
    
    // Is the cycle results animating
    bool isCycleResultsAnimating();

protected:
    
    // Slot group model
    const std::shared_ptr<CSlotGroupModel> m_spSlotGroupModel;
    
    // Spin profile map
    std::map<const std::string, std::vector<CSpinProfile>> m_spinProfileMapVec;
    
    // Default spin profile
    std::string m_defaultSpinProfile;
    
    // Cycle result symb vectors
    std::vector<std::vector<CSymbol2d *>> m_cycleResultSymbVec;
    
    // cycle results text sprite
    std::unique_ptr<CSprite> m_upCycleResultsTxtSprite;
    
    // Cycle results interface
    std::unique_ptr<iCycleResults> m_upCycleResults;
};

#endif
