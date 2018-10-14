
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
    
    // Do any pre-game loop init's
    virtual void init() override;

    // Update the actors
    virtual void update() override;

    // Transform the actor
    virtual void transform() override;
    virtual void transform( const CObject2D & object ) override;

    // Render the actors
    void render() override;
    void render( const CMatrix & matrix ) override;
    void render( const CMatrix & matrix, const CMatrix & rotMatrix ) override;
    
    // Find if the sprite exists
    bool find( CSprite * pSprite );
    
    // Get the default camera position
    CObject & getDefaultCameraPos();
    
    // Get a reference to the sprite
    template <typename target>
    target & get( const std::string & spriteName, int index = 0 )
    {
        return *dynamic_cast<target *>(m_sectorDeq.at(index).get( spriteName ));
    }
    
protected:
    
    // Load the sector data from node
    virtual void loadFromNode( const struct XMLNode & node );
    
protected:

    // Deque of the sector sprites
    std::deque<CSector> m_sectorDeq;
    
    // default camera position
    CObject m_defaultCameraPos;
};

#endif  // __basic_stage_strategy_h__
