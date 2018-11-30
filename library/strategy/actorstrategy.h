
/************************************************************************
*    FILE NAME:       actorstrategy.h
*
*    DESCRIPTION:     Actor strategy class
************************************************************************/

#ifndef __actor_strategy_h__
#define __actor_strategy_h__

// Physical component dependency
#include <strategy/basestrategy.h>

// Game lib dependencies
#include <common/point.h>
#include <common/defs.h>
#include <common/worldvalue.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward Declarations
class CNodeDataList;
class CSpriteData;
class CSprite;
class CObject2D;
class CMatrix;

class CActorStrategy : public CBaseStrategy, boost::noncopyable
{
public:

    // Constructor
    CActorStrategy();

    // Destructor
    virtual ~CActorStrategy();

    // Load the node data from file
    void loadFromFile( const std::string & file ) override;

    // Create the node
    virtual iNode * create(
        const std::string & dataName,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale ) override;

    virtual iNode * create(
        const std::string & dataName ) override;

    // Update the nodes
    void update() override;

    // Transform the node
    void transform() override;

    // Record the command buffer for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, const CCamera & camera ) override;

    // Get the reference to the node
    template<typename target>
    target * get( const int id )
    {
        return dynamic_cast<target *>(getNode( id ));
    }

    // Find if the node is active
    bool isActive( const int id );

protected:
    
    // Load the node
    void load(
        CSprite * pSprite,
        const CSpriteData & rSpriteData,
        const CPoint<CWorldValue> & pos = CPoint<CWorldValue>(),
        const CPoint<float> & rot = CPoint<float>(),
        const CPoint<float> & scale = CPoint<float>(1,1,1) );
    
    void load(
        CObject2D * pObject,
        const CSpriteData & rSpriteData,
        const CPoint<CWorldValue> & pos = CPoint<CWorldValue>(),
        const CPoint<float> & rot = CPoint<float>(),
        const CPoint<float> & scale = CPoint<float>(1,1,1) );

    // Handle the deleting of any nodes
    void deleteObj( const int id ) override;

    // Handle the creating of any object by name
    void createObj( const std::string & name ) override;

    // Get the pointer to the sprite
    iNode * getNode( const int id );
    
    // Get the node data by name
    CNodeDataList & getData( const std::string & name );

protected:

    // Map of the node data
    std::map<const std::string, CNodeDataList> m_dataMap;

    // Vector of iNode pointers
    std::vector<iNode *> m_pNodeVec;
};

#endif


