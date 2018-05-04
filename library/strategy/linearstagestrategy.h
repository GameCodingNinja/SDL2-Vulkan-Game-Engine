
/************************************************************************
*    FILE NAME:       linearstagestrategy.h
*
*    DESCRIPTION:     Linear stage strategy
*                     Strategy is optimized to move in a linear fashion
************************************************************************/

#ifndef __linear_stage_strategy_h__
#define __linear_stage_strategy_h__

// Physical component dependency
#include <strategy/basicstagestrategy.h>

// Game lib dependencies
#include <strategy/sector.h>

// Standard lib dependencies
#include <vector>
#include <deque>

// Forward declaration(s)
class CObject2D;
class CMatrix;

class CLinearStageStrategy : public CBasicStageStrategy
{
public:

    // Constructor
    CLinearStageStrategy();

    // Destructor
    virtual ~CLinearStageStrategy();

    // Update the actors
    void update() override;

    // Transform the actor
    void transform() override;
    void transform( const CObject2D & object ) override;

    // Render the actors
    void render() override;
    void render( const CMatrix & matrix ) override;
    
protected:
    
    // Load thes actor data from node
    virtual void loadFromNode( const XMLNode & node ) override;
    
    // Init the range of sectors to check
    void initRange();

private:
    
    // Set the range based on the sector's visibility
    bool setRange( const size_t index );
    
protected:
    
    enum EStageDirection
    {
        ESD_NULL,
        ESD_FORWARD,
        ESD_BACKWARD,
    };
    
    // Start index
    size_t m_startIndex;
    
    // First sector that is in view
    size_t m_firstIndex;
    
    // Last sector not in view
    size_t m_lastIndex;
    
    // Loop type
    EStageDirection m_dirType;
};

#endif  // __linear_stage_strategy_h__
