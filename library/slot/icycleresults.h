
/************************************************************************
*    FILE NAME:       icycleresult.h
*
*    DESCRIPTION:     Interface for cycle results animations
************************************************************************/

#ifndef __i_cycle_results_h__
#define __i_cycle_results_h__

// Standard lib dependencies
#include <memory>

// Forward declaration(s)
class CSlotGroupView;
class CPlayResult;
class CMatrix;

class iCycleResults
{
public:
    
    // Constructor
    iCycleResults( CPlayResult * pPlayResult );

    // Destructor
    virtual ~iCycleResults();
    
    // Do some inits
    virtual void init( std::shared_ptr<CSlotGroupView> & spSlotGroupView ) = 0;
    
    // Start the cycle results animation
    virtual void startAnimation() = 0;
    
    // Stop the cycle results animation
    virtual void stopAnimation() = 0;
    
    // Are we still animating
    virtual bool isAnimating() = 0;
    
    // Update the cycle results
    virtual void update(){};
    
    // Transform the reel group
    virtual void transform( const CMatrix & matrix, bool tranformWorldPos ){};

    // Do the render
    virtual void render( const CMatrix & matrix ){}
    
    // Activate the cycle results
    virtual void activate();
    
    // Deactivate the cycle results
    virtual void deactivate();
    
    // Did we complete one animation cycle of all the pays?
    bool isFirstCycleComplete();
    
    // Is the cycle results active?
    bool isCycleResultsActive();
    
protected:
    
    // Pointer to the play result. NOTE: We don't own this pointer
    CPlayResult * m_pPlayResult;
    
    // Pointer to slot group view.
    std::shared_ptr<CSlotGroupView> m_spSlotGroupView;
    
    // index into pay
    int m_cyclePayCounter;
    
    // Current cycle pay
    int m_curPayIndex;
    
    // Did we complete one animation cycle of all the pays
    bool m_firstCycleComplete;
    
    // Cycle results flag
    bool m_cycleResultsActive;

};

#endif  // __i_cycle_results_h__


