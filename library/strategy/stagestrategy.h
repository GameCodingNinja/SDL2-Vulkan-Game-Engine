
/************************************************************************
*    FILE NAME:       stagestrategy.h
*
*    DESCRIPTION:     Stage strategy
************************************************************************/

#ifndef __stage_strategy_h__
#define __stage_strategy_h__

// Physical component dependency
#include <strategy/istrategy.h>

// Game lib dependencies
#include <strategy/sector.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <vector>
#include <deque>

// Forward Declarations
class CObject2D;
class CMatrix;

class CStageStrategy : public iStrategy, boost::noncopyable
{
public:

    // Constructor
    CStageStrategy();

    // Destructor
    virtual ~CStageStrategy();

    // Load the sector data from file
    virtual void loadFromFile( const std::string & file ) override;

    // Update the actors
    virtual void update() override;

    // Transform the actor
    virtual void transform() override;
    
    // Record the command buffer for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index ) override;
    
protected:
    
    // Load the sector data from node
    virtual void loadFromNode( const struct XMLNode & node );
    
protected:

    // Deque of the sector sprites
    std::deque<CSector> m_sectorDeq;
};

#endif
