
/************************************************************************
*    FILE NAME:       simplecycleresults.h
*
*    DESCRIPTION:     Simple cycle results implementation
************************************************************************/

#ifndef __simple_cycle_results_h__
#define __simple_cycle_results_h__

// Physical component dependency
#include <slot/icycleresults.h>

// Game lib dependencies
#include <utilities/timer.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

class CSimpleCycleresults : public iCycleResults, boost::noncopyable
{
public:

    // Constructor
    CSimpleCycleresults( CPlayResult * pPlayResult );

    // Destructor
    virtual ~CSimpleCycleresults();
    
    // Do some inits
    void init( std::shared_ptr<CSlotGroupView> & spSlotGroupView ) override;
    
    // Activate the cycle results
    void activate() override;
    
    // Deactivate the cycle results
    void deactivate() override;
    
    // Start the cycle results animation
    void startAnimation() override;
    
    // Stop the cycle results animation
    void stopAnimation() override;
    
    // Animate the cycle results
    bool isAnimating() override;
    
private:

    // cycle results timer
    CTimer m_cycleResultsTimer;
};

#endif
