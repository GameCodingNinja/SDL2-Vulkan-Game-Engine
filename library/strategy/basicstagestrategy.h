
/************************************************************************
*    FILE NAME:       basicstagestrategy.h
*
*    DESCRIPTION:     Basic stage strategy
************************************************************************/

#ifndef __basic_stage_strategy_h__
#define __basic_stage_strategy_h__

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

class CBasicStageStrategy : public iStrategy, boost::noncopyable
{
public:

    // Constructor
    CBasicStageStrategy();

    // Destructor
    virtual ~CBasicStageStrategy();

    // Load the sector data from file
    virtual void loadFromFile( const std::string & file ) override;

    // Update the actors
    virtual void update() override;

    // Transform the actor
    virtual void transform() override;
    
    // Record the command buffer for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, const CCamera & camera ) override;
    
    // Get the default camera position
    CObject & getDefaultCameraPos();
    
protected:
    
    // Load the sector data from node
    virtual void loadFromNode( const struct XMLNode & node );
    
protected:

    // Deque of the sector sprites
    std::deque<CSector> m_sectorDeq;
    
    // default camera position
    CObject m_defaultCameraPos;
};

#endif
