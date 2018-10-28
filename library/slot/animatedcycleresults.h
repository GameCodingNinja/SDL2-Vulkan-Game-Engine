
/************************************************************************
*    FILE NAME:       animatedcycleresults.h
*
*    DESCRIPTION:     Animated cycle results implementation
************************************************************************/

#ifndef __animated_cycle_results_h__
#define __animated_cycle_results_h__

// Physical component dependency
#include <slot/icycleresults.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

class CAnimatedCycleResults : public iCycleResults, boost::noncopyable
{
public:

    // Constructor
    CAnimatedCycleResults( CPlayResult * pPlayResult );

    // Destructor
    virtual ~CAnimatedCycleResults();
    
    // Do some inits
    void init( std::shared_ptr<CSlotGroupView> & spSlotGroupView ) override;
    
    // Update the cycle results
    void update() override;
    
    // Transform
    void transform( const CMatrix & matrix, bool tranformWorldPos ) override;
    
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

    // Do the render
    //void render( const CMatrix & matrix ) override;
};

#endif
