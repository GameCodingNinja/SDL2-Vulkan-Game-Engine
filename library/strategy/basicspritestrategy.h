
/************************************************************************
*    FILE NAME:       basicspritestrategy.h
*
*    DESCRIPTION:     Basic sprite strategy class
************************************************************************/

#ifndef __basic_sprite_strategy_h__
#define __basic_sprite_strategy_h__

// Physical component dependency
#include <strategy/basestrategy.h>

// Game lib dependencies
#include <common/point.h>
#include <common/defs.h>
#include <common/worldvalue.h>
#include <common/inode.h>
#include <sprite/spritedata.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward Declarations
class CSprite;
class CMatrix;
class iNode;

class CBasicSpriteStrategy : public CBaseStrategy, boost::noncopyable
{
public:

    // Constructor
    CBasicSpriteStrategy();

    // Destructor
    virtual ~CBasicSpriteStrategy();

    // Load the sprite data from file
    void loadFromFile( const std::string & file ) override;

    // Create the sprite
    virtual iNode * create(
        const std::string & dataName,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale ) override;

    virtual iNode * create(
        const std::string & dataName ) override;

    // Update the sprites
    void update() override;

    // Transform the sprite
    void transform() override;

    // Render the sprites
    void render() override;
    void render( const CMatrix & matrix ) override;
    void render( const CMatrix & matrix, const CMatrix & rotMatrix ) override;

    // Get the reference to the sprite
    /*template<typename target>
    target & get( const int id )
    {
        return *dynamic_cast<target *>(getSprite( id ));
    }

    // Find if the sprite exists
    bool find( CSprite * piSprite );*/

    // Get the sprite data by name
    CSpriteData & getData( const std::string & name );

protected:
    
    // Load the sprite
    void loadSprite(
        CSprite & sprite,
        const CSpriteData & rSpriteData,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale );

    // Handle the deleting of any sprites
    void deleteObj( int index ) override;

    // Handle the creating of any object by name
    void createObj( const std::string & name ) override;

    // Get the pointer to the sprite
    //CSprite * getSprite( const int id );

protected:

    // Map of the sprite data
    std::map<const std::string, CSpriteData> m_dataMap;

    // Map of all the iNode pointers
    std::map<const int, iNode *> m_pNodeMap;

    // Vector of iNode pointers
    std::vector<iNode *> m_pNodeVec;
};

#endif  // __basic_sprite_strategy_h__


